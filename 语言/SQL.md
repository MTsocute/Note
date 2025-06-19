#  SQL

---

## 1. 查

```sql
# 条件查询
select * from employee where sex = 1 and username = 'momo';

# 模糊查询
select * from employee where age like '%3%';

# 分页 [start from <0 as first one>, how many data]
select * from employee limit 0,2;

# 分组: 统计员工中 ID 是 721 的男性个数是多少
# having 只可以是 sex 里面有的不可以是别的
select count(*) from employee e
where employID = 101
group by sex
having sex = 1;


# 关联查询 + 分页
select employee.*,
       department.name as departmentName
from employee
         left join department # 这个left 是 department 这个表在相对的左边
                   on employee.department_id = department.id
limit 0,2;
```

---

### 1.1 关联查询

> 假定你有如下两个表

| id   | name |
| ---- | ---- |
| 1    | 张三 |
| 2    | 李四 |
| 3    | 王五 |

| student_id | score |
| ---------- | ----- |
| 1          | 95    |
| 2          | 88    |
| 4          | 70    |

> 你想把这两个表连接起来，得到**学生名字和成绩**，可以这样：
>
> ```sql
> SELECT students.name, scores.score
> FROM students
> INNER JOIN scores
> ON students.id = scores.student_id;
> ```

| name | score |
| ---- | ----- |
| 张三 | 95    |
| 李四 | 88    |

JOIN` 默认等于 `INNER JOIN

如果你想要**严格匹配两边都存在的数据**，用 `INNER JOIN`

如果你想要**保留某一边所有数据**，再考虑 `LEFT JOIN`、`RIGHT JOIN`

---

## 2. 增

```sql
INSERT INTO ts_demo.department (id, name) 
VALUES (3, 'purchase_part');
```

## 3. 改

```sql
UPDATE ts_demo.department 
SET name = 'purchase_part' 
WHERE id = 3;
```

## 4. 删

```sql
DELETE  
FROM ts_demo.department
WHERE id = 3;
```

