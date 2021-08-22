# API - 42Labs

* [Sobre o projeto](#sobre-o-projeto)
* [Ferramentas e bibliotecas utilizadas](#ferramentas-e-bibliotecas-utilizadas)
* [Modo de uso](#modo-de-uso)
* [Informações retornadas](#informações-retornadas)
* [Funcionamento](#funcionamento)

## **Sobre o projeto**
Esse projeto consiste em uma API para uma aplicação de análise estátistica de alunos da 42. 
Após a aplicação receber o `login` do aluno, deve retornar informações e análises buscadas na [API oficial da Intra](https://api.intra.42.fr/). Essas informações são colocadas em um banco de dados.

- Para informações sobre o funcionamento da API, acesse: [Funcionamento](#funcionamento)

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

### Modo 2: Acesso através da AmazonEC2
*Amazon Elastic Compute Cloud, ou EC2 permite que usuários aluguem computadores virtuais para rodarem suas próprias aplicações*
- 1-) Entre na seguinte url, substituindo `login` pelo login de um aluno:
```
http://18.234.230.231:3000/login
```

## **Base de dados**
* Toda vez que inserimos um `login` diferente, alguns dados são armazenados na base de dados (mySQL).  Os dados são salvados em duas tabelas diferentes. A tabela *user* armazena dados pessoais, enquanto a tabela *stats* armazena as estatísticas do usuário. Elas tem o *id* como **Primary Key**.
* Para verificar esse processo, siga os passos abaixo:
	* [Siga os passos do Modo 1](#modo-1:-acesso-local)
	- Execute o container docker mysql
	```
	docker exec -it mysql_container /bin/bash
	```
	- Acesse o mySQL
	```
	mysql -u root -p
	```
	- Insira a senha: **password**
	- Selecione a base de dados *api*
	```
	USE api
	```
	- Selecione os dados da tabela *user* ou *stats*
	```
	SELECT * FROM user
	```
	```
	SELECT * FROM stats
	```
	Para parar os containers, execute:
	```
	docker-compose stop
	```

## **Informações retornadas**
**Todas as informações são com base na última atualização**
- user:
	- Login
	- Id
	- Nome completo
	- Email
	- Cidade onde estuda
	- País onde estuda
- stats:
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
Para informações sobre o funcionamento da API, acesse:
* [Documentação da API](https://github.com/42sp/42labs-selection-process-marco-kraemer/blob/main/DOCUMENTATION.md)