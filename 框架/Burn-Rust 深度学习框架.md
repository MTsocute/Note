# Burn-Rust 深度学习框架

---

<br>

## Tensor

> Tensor 结构有 3 个泛型参数：`后端B`、`维度D` 和 `数据类型`

```rust
Tensor<B, D>           // Float tensor (default)
Tensor<B, D, Float>    // Explicit float tensor
Tensor<B, D, Int>      // Int tensor
Tensor<B, D, Bool>     // Bool tensor
```

> [!warning]
>
> **在 Burn 中，张量（Tensor）的维度 D 是在类型声明时固定的，而不是由数据的长度或形状决定的**。也就是说，你在写 `Tensor::<Backend, D>` 时，`D` 表示这个张量的 **维度（rank）**，而不是元素的数量。

```rust
let floats = [1.0, 2.0, 3.0, 4.0, 5.0];

// Get the default device
let device = Default::default();

// correct: Tensor is 1-Dimensional with 5 elements
let tensor_1 = Tensor::<Backend, 1>::from_floats(floats, &device);

// incorrect: let tensor_1 = Tensor::<Backend, 5>::from_floats(floats, &device);
// this will lead to an error and is for creating a 5-D tensor
```

### [Initialization](https://burn.dev/books/burn/building-blocks/tensor.html#initialization) 初始化

> 在 Burn 中，张量主要通过 `from_data()` 方法初始化。这个方法接收一个 `TensorData` 结构体。`TensorData` 有两个公开字段：`shape` 和 `dtype`。数据 `value` 目前是以字节形式存储的，是私有的，但可以通过 `as_slice`、`as_mut_slice`、`to_vec` 和 `iter` 等方法访问。
>
> 如果你想从张量中获取数据：
>
> - 想 **之后还继续使用这个张量**，请用 `.to_data()` 方法；
> - 想 **一次性取出使用**，推荐用 `.into_data()` 方法。
>
> 接下来，我们可以看几个 **从不同输入初始化张量** 的例子。

```rust

// Initialization from a given Backend (Wgpu)
let tensor_1 = Tensor::<Wgpu, 1>::from_data([1.0, 2.0, 3.0], &device);

// Initialization from a generic Backend
let tensor_2 = Tensor::<Backend, 1>::from_data(TensorData::from([1.0, 2.0, 3.0]), &device);
  
// Initialization using from_floats (Recommended for f32 ElementType)
// Will be converted to TensorData internally.
let tensor_3 = Tensor::<Backend, 1>::from_floats([1.0, 2.0, 3.0], &device);

// Initialization of Int Tensor from array slices
let arr: [i32; 6] = [1, 2, 3, 4, 5, 6];
let tensor_4 = Tensor::<Backend, 1, Int>::from_data(TensorData::from(&arr[0..3]), &device);

// Initialization from a custom type

struct BodyMetrics {
    age: i8,
    height: i16,
    weight: f32
}

let bmi = BodyMetrics{
        age: 25,
        height: 180,
        weight: 80.0
};
let data = TensorData::from([bmi.age as f32, bmi.height as f32, bmi.weight]);
let tensor_5 = Tensor::<Backend, 1>::from_data(data, &device);

```



### 所有权问题

> 几乎所有 Burn `操作` 都会拥有输入张量的所有权。所以，重复使用一个张量就需要克隆它

```rust
let input = Tensor::<Wgpu, 1>::from_floats([1.0, 2.0, 3.0, 4.0], &device);
let min = input.min();		// .min() 方法使用之后，input 就不可以用了
let max = input.max();		// 这里如果用 .max() 就会有所有权问题
let input = (input - min).div(max - min);
```

> 解决方法

```rust
let input = Tensor::<Wgpu, 1>::from_floats([1.0, 2.0, 3.0, 4.0], &device);
let min = input.clone().min();
let max = input.clone().max();
let input = (input.clone() - min.clone()).div(max - min);	// 这里递交了所有权
println!("{}", input.to_data());// Success: [0.0, 0.33333334, 0.6666667, 1.0]

// Notice that max, min have been moved in last operation so
// the below print will give an error.
// If we want to use them for further operations,
// they will need to be cloned in similar fashion.
// println!("{:?}", min.to_data());
```

<br>

### [Tensor Operations](https://burn.dev/books/burn/building-blocks/tensor.html#tensor-operations) [张量运算](https://burn.dev/books/burn/building-blocks/tensor.html#tensor-operations)

> 这里列出了所有支持的运算及其等价对应 PyTorch 的哪些操作。这些操作适用于所有张量类型：`整型`、`浮点型` 和 `布尔型`
>
> 注意啦，为了简单，我们没考虑类型签名。想了解更多，看 [完整文档](https://docs.rs/burn/latest/burn/tensor/struct.Tensor.html) 去。



#### [Basic Operations](https://burn.dev/books/burn/building-blocks/tensor.html#basic-operations)

Those operations are available for all tensor kinds: `Int`, `Float`, and `Bool`.

| Burn                                        |                      PyTorch Equivalent                      |
| :------------------------------------------ | :----------------------------------------------------------: |
| `Tensor::cat(tensors, dim)`                 |                  `torch.cat(tensors, dim)`                   |
| `Tensor::empty(shape, device)`              |             `torch.empty(shape, device=device)`              |
| `Tensor::from_primitive(primitive)`         |                             N/A                              |
| `Tensor::stack(tensors, dim)`               |                 `torch.stack(tensors, dim)`                  |
| `tensor.all()`                              |                        `tensor.all()`                        |
| `tensor.all_dim(dim)`                       |                      `tensor.all(dim)`                       |
| `tensor.any()`                              |                        `tensor.any()`                        |
| `tensor.any_dim(dim)`                       |                      `tensor.any(dim)`                       |
| `tensor.chunk(num_chunks, dim)`             |               `tensor.chunk(num_chunks, dim)`                |
| `tensor.split(split_size, dim)`             |               `tensor.split(split_size, dim)`                |
| `tensor.split_with_sizes(split_sizes, dim)` |              `tensor.split([split_sizes], dim)`              |
| `tensor.device()`                           |                       `tensor.device`                        |
| `tensor.dtype()`                            |                        `tensor.dtype`                        |
| `tensor.dims()`                             |                       `tensor.size()`                        |
| `tensor.equal(other)`                       |                           `x == y`                           |
| `tensor.expand(shape)`                      |                    `tensor.expand(shape)`                    |
| `tensor.flatten(start_dim, end_dim)`        |             `tensor.flatten(start_dim, end_dim)`             |
| `tensor.flip(axes)`                         |                     `tensor.flip(axes)`                      |
| `tensor.into_data()`                        |                             N/A                              |
| `tensor.into_primitive()`                   |                             N/A                              |
| `tensor.into_scalar()`                      |                       `tensor.item()`                        |
| `tensor.narrow(dim, start, length)`         |             `tensor.narrow(dim, start, length)`              |
| `tensor.not_equal(other)`                   |                           `x != y`                           |
| `tensor.permute(axes)`                      |                    `tensor.permute(axes)`                    |
| `tensor.movedim(src, dst)`                  |                  `tensor.movedim(src, dst)`                  |
| `tensor.repeat_dim(dim, times)`             | `tensor.repeat(*[times if i == dim else 1 for i in range(tensor.dim())])` |
| `tensor.repeat(sizes)`                      |                    `tensor.repeat(sizes)`                    |
| `tensor.reshape(shape)`                     |                     `tensor.view(shape)`                     |
| `tensor.roll(shfts, dims)`                  |                 `tensor.roll(shifts, dims)`                  |
| `tensor.roll_dim(shift, dim)`               |                `tensor.roll([shift], [dim])`                 |
| `tensor.shape()`                            |                        `tensor.shape`                        |
| `tensor.slice(ranges)`                      |                     `tensor[(*ranges,)]`                     |
| `tensor.slice_assign(ranges, values)`       |                `tensor[(*ranges,)] = values`                 |
| `tensor.slice_fill(ranges, value)`          |                 `tensor[(*ranges,)] = value`                 |
| `tensor.slice_dim(dim, range)`              |                             N/A                              |
| `tensor.squeeze(dim)`                       |                    `tensor.squeeze(dim)`                     |
| `tensor.swap_dims(dim1, dim2)`              |                `tensor.transpose(dim1, dim2)`                |
| `tensor.to_data()`                          |                             N/A                              |
| `tensor.to_device(device)`                  |                     `tensor.to(device)`                      |
| `tensor.transpose()`                        |                          `tensor.T`                          |
| `tensor.unsqueeze()`                        |                    `tensor.unsqueeze(0)`                     |
| `tensor.unsqueeze_dim(dim)`                 |                   `tensor.unsqueeze(dim)`                    |
| `tensor.unsqueeze_dims(dims)`               |                             N/A                              |

#### [Numeric Operations](https://burn.dev/books/burn/building-blocks/tensor.html#numeric-operations)

Those operations are available for numeric tensor kinds: `Float` and `Int`.

| Burn                                                         | PyTorch Equivalent                             |
| ------------------------------------------------------------ | ---------------------------------------------- |
| `Tensor::eye(size, device)`                                  | `torch.eye(size, device=device)`               |
| `Tensor::full(shape, fill_value, device)`                    | `torch.full(shape, fill_value, device=device)` |
| `Tensor::ones(shape, device)`                                | `torch.ones(shape, device=device)`             |
| `Tensor::zeros(shape, device)`                               | `torch.zeros(shape, device=device)`            |
| `tensor.abs()`                                               | `torch.abs(tensor)`                            |
| `tensor.add(other)` or `tensor + other`                      | `tensor + other`                               |
| `tensor.add_scalar(scalar)` or `tensor + scalar`             | `tensor + scalar`                              |
| `tensor.all_close(other, atol, rtol)`                        | `torch.allclose(tensor, other, atol, rtol)`    |
| `tensor.argmax(dim)`                                         | `tensor.argmax(dim)`                           |
| `tensor.argmin(dim)`                                         | `tensor.argmin(dim)`                           |
| `tensor.argsort(dim)`                                        | `tensor.argsort(dim)`                          |
| `tensor.argsort_descending(dim)`                             | `tensor.argsort(dim, descending=True)`         |
| `tensor.bool()`                                              | `tensor.bool()`                                |
| `tensor.clamp(min, max)`                                     | `torch.clamp(tensor, min=min, max=max)`        |
| `tensor.clamp_max(max)`                                      | `torch.clamp(tensor, max=max)`                 |
| `tensor.clamp_min(min)`                                      | `torch.clamp(tensor, min=min)`                 |
| `tensor.contains_nan()`                                      | N/A                                            |
| `tensor.div(other)` or `tensor / other`                      | `tensor / other`                               |
| `tensor.div_scalar(scalar)` or `tensor / scalar`             | `tensor / scalar`                              |
| `tensor.equal_elem(other)`                                   | `tensor.eq(other)`                             |
| `tensor.full_like(fill_value)`                               | `torch.full_like(tensor, fill_value)           |
| `tensor.gather(dim, indices)`                                | `torch.gather(tensor, dim, indices)`           |
| `tensor.greater(other)`                                      | `tensor.gt(other)`                             |
| `tensor.greater_elem(scalar)`                                | `tensor.gt(scalar)`                            |
| `tensor.greater_equal(other)`                                | `tensor.ge(other)`                             |
| `tensor.greater_equal_elem(scalar)`                          | `tensor.ge(scalar)`                            |
| `tensor.lower(other)`                                        | `tensor.lt(other)`                             |
| `tensor.lower_elem(scalar)`                                  | `tensor.lt(scalar)`                            |
| `tensor.lower_equal(other)`                                  | `tensor.le(other)`                             |
| `tensor.lower_equal_elem(scalar)`                            | `tensor.le(scalar)`                            |
| `tensor.mask_fill(mask, value)`                              | `tensor.masked_fill(mask, value)`              |
| `tensor.mask_where(mask, value_tensor)`                      | `torch.where(mask, value_tensor, tensor)`      |
| `tensor.max()`                                               | `tensor.max()`                                 |
| `tensor.max_abs()`                                           | `tensor.abs().max()`                           |
| `tensor.max_abs_dim(dim)`                                    | `tensor.abs().max(dim, keepdim=True)`          |
| `tensor.max_dim(dim)`                                        | `tensor.max(dim, keepdim=True)`                |
| `tensor.max_dim_with_indices(dim)`                           | N/A                                            |
| `tensor.max_pair(other)`                                     | `torch.Tensor.max(a,b)`                        |
| `tensor.mean()`                                              | `tensor.mean()`                                |
| `tensor.mean_dim(dim)`                                       | `tensor.mean(dim, keepdim=True)`               |
| `tensor.min()`                                               | `tensor.min()`                                 |
| `tensor.min_dim(dim)`                                        | `tensor.min(dim, keepdim=True)`                |
| `tensor.min_dim_with_indices(dim)`                           | N/A                                            |
| `tensor.min_pair(other)`                                     | `torch.Tensor.min(a,b)`                        |
| `tensor.mul(other)` or `tensor * other`                      | `tensor * other`                               |
| `tensor.mul_scalar(scalar)` or `tensor * scalar`             | `tensor * scalar`                              |
| `tensor.neg()` or `-tensor`                                  | `-tensor`                                      |
| `tensor.not_equal_elem(scalar)`                              | `tensor.ne(scalar)`                            |
| `tensor.ones_like()`                                         | `torch.ones_like(tensor)`                      |
| `tensor.one_hot(num_classes)`                                | `torch.nn.functional.one_hot`                  |
| `tensor.one_hot_fill(num_classes, on_value, off_value, axis)` | N/A                                            |
| `tensor.pad(pads, value)`                                    | `torch.nn.functional.pad(input, pad, value)`   |
| `tensor.powf(other)` or `tensor.powi(intother)`              | `tensor.pow(other)`                            |
| `tensor.powf_scalar(scalar)` or `tensor.powi_scalar(intscalar)` | `tensor.pow(scalar)`                           |
| `tensor.prod()`                                              | `tensor.prod()`                                |
| `tensor.prod_dim(dim)`                                       | `tensor.prod(dim, keepdim=True)`               |
| `tensor.rem(other)` or `tensor % other`                      | `tensor % other`                               |
| `tensor.scatter(dim, indices, values)`                       | `tensor.scatter_add(dim, indices, values)`     |
| `tensor.select(dim, indices)`                                | `tensor.index_select(dim, indices)`            |
| `tensor.select_assign(dim, indices, values)`                 | N/A                                            |
| `tensor.sign()`                                              | `tensor.sign()`                                |
| `tensor.sort(dim)`                                           | `tensor.sort(dim).values`                      |
| `tensor.sort_descending(dim)`                                | `tensor.sort(dim, descending=True).values`     |
| `tensor.sort_descending_with_indices(dim)`                   | `tensor.sort(dim, descending=True)`            |
| `tensor.sort_with_indices(dim)`                              | `tensor.sort(dim)`                             |
| `tensor.sub(other)` or `tensor - other`                      | `tensor - other`                               |
| `tensor.sub_scalar(scalar)` or `tensor - scalar`             | `tensor - scalar`                              |
| `scalar - tensor`                                            | `scalar - tensor`                              |
| `tensor.sum()`                                               | `tensor.sum()`                                 |
| `tensor.sum_dim(dim)`                                        | `tensor.sum(dim, keepdim=True)`                |
| `tensor.topk(k, dim)`                                        | `tensor.topk(k, dim).values`                   |
| `tensor.topk_with_indices(k, dim)`                           | `tensor.topk(k, dim)`                          |
| `tensor.tril(diagonal)`                                      | `torch.tril(tensor, diagonal)`                 |
| `tensor.triu(diagonal)`                                      | `torch.triu(tensor, diagonal)`                 |
| `tensor.zeros_like()`                                        | `torch.zeros_like(tensor)`                     |

#### [Float Operations](https://burn.dev/books/burn/building-blocks/tensor.html#float-operations)

Those operations are only available for `Float` tensors.

| Burn API                                     | PyTorch Equivalent                         |
| -------------------------------------------- | ------------------------------------------ |
| `tensor.cast(dtype)`                         | `tensor.to(dtype)`                         |
| `tensor.ceil()`                              | `tensor.ceil()`                            |
| `tensor.cos()`                               | `tensor.cos()`                             |
| `tensor.cosh()`                              | `tensor.cosh()`                            |
| `tensor.erf()`                               | `tensor.erf()`                             |
| `tensor.exp()`                               | `tensor.exp()`                             |
| `tensor.floor()`                             | `tensor.floor()`                           |
| `tensor.from_floats(floats, device)`         | N/A                                        |
| `tensor.from_full_precision(tensor)`         | N/A                                        |
| `tensor.int()`                               | Similar to `tensor.to(torch.long)`         |
| `tensor.is_close(other, atol, rtol)`         | `torch.isclose(tensor, other, atol, rtol)` |
| `tensor.is_finite()`                         | `torch.isfinite(tensor)`                   |
| `tensor.is_inf()`                            | `torch.isinf(tensor)`                      |
| `tensor.is_nan()`                            | `torch.isnan(tensor)`                      |
| `tensor.log()`                               | `tensor.log()`                             |
| `tensor.log1p()`                             | `tensor.log1p()`                           |
| `tensor.matmul(other)`                       | `tensor.matmul(other)`                     |
| `tensor.random(shape, distribution, device)` | N/A                                        |
| `tensor.random_like(distribution)`           | `torch.rand_like()` only uniform           |
| `tensor.recip()` or `1.0 / tensor`           | `tensor.reciprocal()` or `1.0 / tensor`    |
| `tensor.round()`                             | `tensor.round()`                           |
| `tensor.sin()`                               | `tensor.sin()`                             |
| `tensor.sinh()`                              | `tensor.sinh()`                            |
| `tensor.sqrt()`                              | `tensor.sqrt()`                            |
| `tensor.tan()`                               | `tensor.tan()`                             |
| `tensor.tanh()`                              | `tensor.tanh()`                            |
| `tensor.to_full_precision()`                 | `tensor.to(torch.float)`                   |
| `tensor.var(dim)`                            | `tensor.var(dim)`                          |
| `tensor.var_bias(dim)`                       | N/A                                        |
| `tensor.var_mean(dim)`                       | N/A                                        |
| `tensor.var_mean_bias(dim)`                  | N/A                                        |

#### [Int Operations](https://burn.dev/books/burn/building-blocks/tensor.html#int-operations)

Those operations are only available for `Int` tensors.

| Burn API                                         | PyTorch Equivalent                                      |
| ------------------------------------------------ | ------------------------------------------------------- |
| `Tensor::arange(5..10, device)`                  | `tensor.arange(start=5, end=10, device=device)`         |
| `Tensor::arange_step(5..10, 2, device)`          | `tensor.arange(start=5, end=10, step=2, device=device)` |
| `tensor.bitwise_and(other)`                      | `torch.bitwise_and(tensor, other)`                      |
| `tensor.bitwise_and_scalar(scalar)`              | `torch.bitwise_and(tensor, scalar)`                     |
| `tensor.bitwise_not()`                           | `torch.bitwise_not(tensor)`                             |
| `tensor.bitwise_left_shift(other)`               | `torch.bitwise_left_shift(tensor, other)`               |
| `tensor.bitwise_left_shift_scalar(scalar)`       | `torch.bitwise_left_shift(tensor, scalar)`              |
| `tensor.bitwise_right_shift(other)`              | `torch.bitwise_right_shift(tensor, other)`              |
| `tensor.bitwise_right_shift_scalar(scalar)`      | `torch.bitwise_right_shift(tensor, scalar)`             |
| `tensor.bitwise_or(other)`                       | `torch.bitwise_or(tensor, other)`                       |
| `tensor.bitwise_or_scalar(scalar)`               | `torch.bitwise_or(tensor, scalar)`                      |
| `tensor.bitwise_xor(other)`                      | `torch.bitwise_xor(tensor, other)`                      |
| `tensor.bitwise_xor_scalar(scalar)`              | `torch.bitwise_xor(tensor, scalar)`                     |
| `tensor.float()`                                 | `tensor.to(torch.float)`                                |
| `tensor.from_ints(ints)`                         | N/A                                                     |
| `tensor.int_random(shape, distribution, device)` | N/A                                                     |
| `tensor.cartesian_grid(shape, device)`           | N/A                                                     |

#### [Bool Operations](https://burn.dev/books/burn/building-blocks/tensor.html#bool-operations)

Those operations are only available for `Bool` tensors.

| Burn API                             | PyTorch Equivalent              |
| ------------------------------------ | ------------------------------- |
| `Tensor::diag_mask(shape, diagonal)` | N/A                             |
| `Tensor::tril_mask(shape, diagonal)` | N/A                             |
| `Tensor::triu_mask(shape, diagonal)` | N/A                             |
| `tensor.argwhere()`                  | `tensor.argwhere()`             |
| `tensor.bool_and()`                  | `tensor.logical_and()`          |
| `tensor.bool_not()`                  | `tensor.logical_not()`          |
| `tensor.bool_or()`                   | `tensor.logical_or()`           |
| `tensor.float()`                     | `tensor.to(torch.float)`        |
| `tensor.int()`                       | `tensor.to(torch.long)`         |
| `tensor.nonzero()`                   | `tensor.nonzero(as_tuple=True)` |

#### [Quantization Operations](https://burn.dev/books/burn/building-blocks/tensor.html#quantization-operations)

Those operations are only available for `Float` tensors on backends that implement quantization strategies.

| Burn API                           | PyTorch Equivalent |
| ---------------------------------- | ------------------ |
| `tensor.quantize(scheme, qparams)` | N/A                |
| `tensor.dequantize()`              | N/A                |

### [Activation Functions](https://burn.dev/books/burn/building-blocks/tensor.html#activation-functions)

| Burn API                                         | PyTorch Equivalent                                 |
| ------------------------------------------------ | -------------------------------------------------- |
| `activation::gelu(tensor)`                       | `nn.functional.gelu(tensor)`                       |
| `activation::hard_sigmoid(tensor, alpha, beta)`  | `nn.functional.hardsigmoid(tensor)`                |
| `activation::leaky_relu(tensor, negative_slope)` | `nn.functional.leaky_relu(tensor, negative_slope)` |
| `activation::log_sigmoid(tensor)`                | `nn.functional.log_sigmoid(tensor)`                |
| `activation::log_softmax(tensor, dim)`           | `nn.functional.log_softmax(tensor, dim)`           |
| `activation::mish(tensor)`                       | `nn.functional.mish(tensor)`                       |
| `activation::prelu(tensor,alpha)`                | `nn.functional.prelu(tensor,weight)`               |
| `activation::quiet_softmax(tensor, dim)`         | `nn.functional.quiet_softmax(tensor, dim)`         |
| `activation::relu(tensor)`                       | `nn.functional.relu(tensor)`                       |
| `activation::sigmoid(tensor)`                    | `nn.functional.sigmoid(tensor)`                    |
| `activation::silu(tensor)`                       | `nn.functional.silu(tensor)`                       |
| `activation::softmax(tensor, dim)`               | `nn.functional.softmax(tensor, dim)`               |
| `activation::softmin(tensor, dim)`               | `nn.functional.softmin(tensor, dim)`               |
| `activation::softplus(tensor, beta)`             | `nn.functional.softplus(tensor, beta)`             |
| `activation::tanh(tensor)`                       | `nn.functional.tanh(tensor)`                       |

### [Grid Functions](https://burn.dev/books/burn/building-blocks/tensor.html#grid-functions)

| Burn API                                           | PyTorch Equivalent                       |
| -------------------------------------------------- | ---------------------------------------- |
| `grid::meshgrid(tensors, GridIndexing::Matrix)`    | `torch.meshgrid(tensors, indexing="ij")` |
| `grid::meshgrid(tensors, GridIndexing::Cartesian)` | `torch.meshgrid(tensors, indexing="xy")` |

### [Linalg Functions](https://burn.dev/books/burn/building-blocks/tensor.html#linalg-functions)

| Burn API                               | PyTorch Equivalent                         |
| -------------------------------------- | ------------------------------------------ |
| `linalg::vector_norm(tensors, p, dim)` | `torch.linalg.vector_norm(tensor, p, dim)` |

<br>

## Module

> `模块推导功能` 让你能自己创建类似 PyTorch 的神经网络模块。这个 `推导函数只生成必要的函数对象`，本质上充当你 **类型的参数容器**，`它对前向传播的声明不做任何代码生成`
>
> 也就是说，你告诉他计算的方式，它默认会生成一个存储你计算方式结果的存储容器

```rust
use burn::module::Module;
use burn::tensor::backend::Backend;

#[derive(Module, Debug)]
pub struct PositionWiseFeedForward<B: Backend> {
    linear_inner: Linear<B>,
    linear_outer: Linear<B>,
    dropout: Dropout,
    gelu: Gelu,
}

```

> [!note]
>
> Burn 就会自动把 `linear_inner`、`linear_outer` 里的参数都“注册”进来，而 `Dropout`、`Gelu` 这种无参数层就自动跳过。
>
> 它 `对前向传播的声明不做任何代码生成`，所以要你自己写这个部分，让 Module 知道正向传播的时候的逻辑是如何走的

```rust
impl<B: Backend> PositionWiseFeedForward<B> {
    pub fn forward<const D: usize>(&self, input: Tensor<B, D>) -> Tensor<B, D> {
        let x = self.linear_inner.forward(input); // 线性
        let x = self.gelu.forward(x);             // 激活
        let x = self.dropout.forward(x);          // Dropout
        self.linear_outer.forward(x)              // 再过一次线性
    }
}
```

### [Visitor & Mapper](https://burn.dev/books/burn/building-blocks/module.html#visitor--mapper)

> 模块主要就是装参数的。所以我们自然提供了多种方法来对每个参数进行操作。
>
> 两个方法都可以对容器参数进行访问，但是后者可以修改值哦

```rust
/// Module visitor trait.
pub trait ModuleVisitor<B: Backend> {
    /// Visit a float tensor in the module.
    fn visit_float<const D: usize>(&mut self, id: ParamId, tensor: &Tensor<B, D>);
    /// Visit an int tensor in the module.
    fn visit_int<const D: usize>(&mut self, id: ParamId, tensor: &Tensor<B, D, Int>);
    /// Visit a bool tensor in the module.
    fn visit_bool<const D: usize>(&mut self, id: ParamId, tensor: &Tensor<B, D, Bool>);
}

/// Module mapper trait.
pub trait ModuleMapper<B: Backend> {
    /// Map a float tensor in the module.
    fn map_float<const D: usize>(&mut self, id: ParamId, tensor: Tensor<B, D>) -> Tensor<B, D>;
    /// Map an int tensor in the module.
    fn map_int<const D: usize>(&mut self, id: ParamId, tensor: Tensor<B, D, Int>) -> Tensor<B, D, Int>;
    /// Map a bool tensor in the module.
    fn map_bool<const D: usize>(&mut self, id: ParamId, tensor: Tensor<B, D, Bool>) -> Tensor<B, D, Bool>;
}
```

#### 用 Mapper 实现一个参数的钳位

> Mapper 是可以修改参数的，所以我们如果参数如果存在范围，可以利用 mapper 做一个钳制的作用

```rust
/// Clamp parameters into the range `[min, max]`.
pub struct Clamp {
    /// Lower-bound of the range.
    pub min: f32,
    /// Upper-bound of the range.
    pub max: f32,
}

// Clamp all floating-point parameter tensors between `[min, max]`.
impl<B: Backend> ModuleMapper<B> for Clamp {
    fn map_float<const D: usize>(
        &mut self,
        _id: burn::module::ParamId,
        tensor: burn::prelude::Tensor<B, D>,
    ) -> burn::prelude::Tensor<B, D> {
        tensor.clamp(self.min, self.max)
    }
}

// Clamp module mapper into the range `[-0.5, 0.5]`
let mut clamp = Clamp {
    min: -0.5,
    max: 0.5,
};

// 模型就会自动 clamp 了，不需要在 forward 再去限制了，很 nice
let model = model.map(&mut clamp);
```

<br>

### Module 的 可存储的类型

#### [General](https://burn.dev/books/burn/building-blocks/module.html#general)

| Burn API        | PyTorch Equivalent                            |
| --------------- | --------------------------------------------- |
| `BatchNorm`     | `nn.BatchNorm1d`, `nn.BatchNorm2d` etc.       |
| `Dropout`       | `nn.Dropout`                                  |
| `Embedding`     | `nn.Embedding`                                |
| `Gelu`          | `nn.Gelu`                                     |
| `GroupNorm`     | `nn.GroupNorm`                                |
| `HardSigmoid`   | `nn.Hardsigmoid`                              |
| `InstanceNorm`  | `nn.InstanceNorm1d`, `nn.InstanceNorm2d` etc. |
| `LayerNorm`     | `nn.LayerNorm`                                |
| `LeakyRelu`     | `nn.LeakyReLU`                                |
| `Linear`        | `nn.Linear`                                   |
| `Prelu`         | `nn.PReLu`                                    |
| `Relu`          | `nn.ReLU`                                     |
| `RmsNorm`       | *No direct equivalent*                        |
| `SwiGlu`        | *No direct equivalent*                        |
| `Interpolate1d` | *No direct equivalent*                        |
| `Interpolate2d` | *No direct equivalent*                        |

#### [Convolutions](https://burn.dev/books/burn/building-blocks/module.html#convolutions)

| Burn API          | PyTorch Equivalent             |
| ----------------- | ------------------------------ |
| `Conv1d`          | `nn.Conv1d`                    |
| `Conv2d`          | `nn.Conv2d`                    |
| `Conv3d`          | `nn.Conv3d`                    |
| `ConvTranspose1d` | `nn.ConvTranspose1d`           |
| `ConvTranspose2d` | `nn.ConvTranspose2d`           |
| `ConvTranspose3d` | `nn.ConvTranspose3d`           |
| `DeformConv2d`    | `torchvision.ops.DeformConv2d` |

#### [Pooling](https://burn.dev/books/burn/building-blocks/module.html#pooling)

| Burn API            | PyTorch Equivalent     |
| ------------------- | ---------------------- |
| `AdaptiveAvgPool1d` | `nn.AdaptiveAvgPool1d` |
| `AdaptiveAvgPool2d` | `nn.AdaptiveAvgPool2d` |
| `AvgPool1d`         | `nn.AvgPool1d`         |
| `AvgPool2d`         | `nn.AvgPool2d`         |
| `MaxPool1d`         | `nn.MaxPool1d`         |
| `MaxPool2d`         | `nn.MaxPool2d`         |

#### [RNNs](https://burn.dev/books/burn/building-blocks/module.html#rnns)

| Burn API         | PyTorch Equivalent     |
| ---------------- | ---------------------- |
| `Gru`            | `nn.GRU`               |
| `Lstm`/`BiLstm`  | `nn.LSTM`              |
| `GateController` | *No direct equivalent* |

#### [Transformer](https://burn.dev/books/burn/building-blocks/module.html#transformer)

| Burn API             | PyTorch Equivalent      |
| -------------------- | ----------------------- |
| `MultiHeadAttention` | `nn.MultiheadAttention` |
| `TransformerDecoder` | `nn.TransformerDecoder` |
| `TransformerEncoder` | `nn.TransformerEncoder` |
| `PositionalEncoding` | *No direct equivalent*  |
| `RotaryEncoding`     | *No direct equivalent*  |

#### [Loss](https://burn.dev/books/burn/building-blocks/module.html#loss)

| Burn API                 | PyTorch Equivalent       |
| ------------------------ | ------------------------ |
| `BinaryCrossEntropyLoss` | `nn.BCELoss`             |
| `CosineEmbeddingLoss`    | `nn.CosineEmbeddingLoss` |
| `CrossEntropyLoss`       | `nn.CrossEntropyLoss`    |
| `HuberLoss`              | `nn.HuberLoss`           |
| `MseLoss`                | `nn.MSELoss`             |
| `PoissonNllLoss`         | `nn.PoissonNLLLoss`      |

<br>

## DataSet

```rust
pub trait Dataset<I>: Send + Sync {
    fn get(&self, index: usize) -> Option<I>;
    fn len(&self) -> usize;
}
```

