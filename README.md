# API - 42Labs

## **Sobre o projeto**
Esse projeto consiste em uma API para uma aplicação de análise estátistica de alunos da 42. 
Após a aplicação receber o `login` do aluno, deve retornar informações e análises buscadas na [API oficial da Intra](https://api.intra.42.fr/). Essas informações são colocadas em um banco de dados.

# **Informações sobre a API**

## **Ferramentas e bibliotecas utilizadas**

##### MySQL
- Para criação de um banco de dados
##### Mongoose
- Para a criação de um servidor
##### Json-c e Mjson
- Para lidar com formatos JSON
##### Libcurl
- Para fazer as requisições na [API oficial da Intra](https://api.intra.42.fr)

## **Modo de uso**
### Modo 1: Acesso local
Para iniciar a API basta realizar o comando:

```
docker-compose up -d
```

Após isso, podemos acessar a seguinte url, substituindo `login` pelo login de um aluno, para ter acesso às estatísticas:
```
http://localhost:3000/login
```

Toda vez que inserimos um `login` diferente, alguns dados são armazenados na base de dados (mySQL). Podemos verificar esse processo realizando os seguintes comandos:

- 1-) Execute o container docker mysql
```
docker exec -it mysql_container /bin/bash
```
- 2-) Acesse o mySQL
```
mysql -u root -p
```
- 3-) Insira a senha: **password**
- 4-) Selecione a base de dados *api*
```
USE api
```
- 5-) Selecione os dados da tabela *students*
```
SELECT * FROM students
```
Para parar os containers, execute:
```
docker-compose stop
```

### Modo 2: Acesso via AmazonEC2
*Amazon Elastic Compute Cloud, ou EC2 permite que usuários aluguem computadores virtuais para rodarem suas próprias aplicações*
- 1-) Entre na seguinte url, substituindo `login` pelo login de um aluno:
```
http://18.234.230.231:3000/login
```

## **Informações retornadas**
**Todas as informações são com base na última atualização**
- login
- Pontos de correção
- Carteira
- Aumento médio da carteira por projeto
- Número de projetos feitos
- Total de tentativas
- Maior nota
- Menor nota
- Nota média
- Total de dias na 42
- Dias necessários para finalizar um projeto
- Horas necessárias para finalizar um projeto
- Média de tentativas por projeto
- Level
- Número de conquistas

## **Funcionamento**