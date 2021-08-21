# API - 42Labs

## **Sobre o projeto**
Esse projeto consiste em uma API para uma aplicação de análise estátistica de alunos da 42. 
Após a aplicação receber o `login` do aluno, deve retornar informações e análises buscadas na [API oficial da Intra](https://api.intra.42.fr/). Essas informações são colocadas em um banco de dados.

# **Informações sobre a API**

## **Ferramentas e bibliotecas utilizadas**

##### MySQL
- Para criação de um banco de dados
########Mongoose
- Para a criação de um servidor
##### Json-c e Mjson
- Para lidar com formatos JSON
##### Libcurl
- Para fazer as requisições na [API oficial da Intra](https://api.intra.42.fr)

## **Modo de uso**
Para iniciar a API basta realizar o comando:

```
docker-compose up -d
```

Após isso, podemos acessar a seguinte url, substituindo `login` pelo login de algum aluno, para ter acesso às estatísticas:
```
http:://localhost:3000/login
```

Toda vez que inserimos um `login` diferente, alguns dados são armazenados na base de dados (mySQL). Podemos verificar esse processo realizando os seguintes comandos:

1-) Execute o container docker mysql
```
docker exec -it mysql_container /bin/bash
```
2-) Acesse o mySQL
```
mysql -u root -p
```
3-) Insira a senha que será solicitada: *password*
4-) Selecione a base de dados *api*
```
USE API
```
5-) Selecione os dados da tabela *students*
```
SELECT * FROM students
```