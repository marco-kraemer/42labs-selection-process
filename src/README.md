# API

* Método
```
GET
```

* URL
```
http://localhost:3000/login
```
ou
```
http://18.234.230.231:3000/login
```
## Resposta em caso de sucesso
```
{
  "login": maraurel,
  "correction_points": 3,
  "wallet": 160,
  "average_wallet_increase_per_project": 6.956522,
  "number_of_projects_done": 23,
  "number_of_attempts": 38,
  "highest_grade": 120,
  "lowest_grade": 0,
  "average_grade": 67,
  "days_in_42": 270,
  "days_finish_a_project": 11.739130,
  "hours_finish_a_project": 281.739136,
  "average_number_of_attempts_per_project": 1.652174,
  "level": 3.58,
  "number_achievements": 21
}
```
* Código 200 - OK

## Resposta em caso de erro
```
{
  "error": 404,
  "message": Student_someuser_notfound
}
```
* Código 404 - Not Found