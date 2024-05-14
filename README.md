# CustomPathfinding
Projeto destinado para criação de um sistema de pathfinding customizado


# Instruções

Caso inicie o projeto e o personagem não se mova, favor verificar no Agente Path, se está adicionado uma curva de float no inspector do PathAgente.

Existe a possibilidade de criar varios grid e transitar entre eles utilizando o pathFinding.

Grid é possivel adiciona-los em qualquer superfice, sendo ela rotacionada ou não, unico ponto de adendo é verificar se tanto a mesh de chão (Ground) e/ou os Obstaculos (Actor de preferencia) estão configurados dentro do grid para seus respectivas variaveis.

## Modulos

Para realizar o PathFinding, foi separado em 3 módulos c++, sendo eles:
1. GridMapping
2. PathFinding
3. CustomPathFinding (Main project)

Cada modulo funciona totalmente idependente, ou seja, da para facilmente adicionar um módulo em outro projeto, sem nenhum problema e o mesmo funcionará com ele mesmo, sem dependencias.

### Grid Mapping

Aqui é onde fica o mapeamento do mundo, criando um grid conforme a area estabelicida pelo actor.

Contem 2 pontos importantes:

1. Grid Component
Aqui é o component do grid, sendo ele o seu grid de fato, podendo ser adicionado em qualquer actor de preferencia, sendo ele novo ou ja contendo funcionalidades.

Caso queira um grid ou adicionar novos, basta entrar no seu actor, e adicionar o "GridComponent" dedntro da filhação do actor

2. Grid Manager
Grid manager é responsável por gerenciar todos os grid existentes no mundo, através dele que conseguirá acessar seu grid de qualquer local de seu projeto, pois o mesmo é um singleton C++.

Grid manager trabalha com métodos estáticos, ou seja, pode ser chamado em qualqquer BP e manipular seu grid atráves dele, além de conseguir trocar para outros grid a partir dele, caso assim seja necessario.

### PathFinding

Contém 3 pontos importantes, sendo eles:
1. Agente Path Component

Responsável por criar a conexão entre seu Pawn (Controlador) e os calculos de buscar o menor caminho.

Além de o Agente Path, ser o intermediário para os calculos de caminho, ele tbm irá realizar o movimento do seu ator dentro deste caminho estabelecido

Para utiliza-lo basta somente adiciona-lo dentro do seu personagem, Pawn e etc como um componente, e após isto, adicionar uma curva de tempo para o movimento pelo Path.

2. Path Search AStar

Aqui é onde contém todos os calculos e pontos para encontrar o menor caminho dentro de seu grid/
mapeamento. Path Search AStar é comente um c++, sem a possibilidade de acessa-lo via BP, para utiliza-lo, é necessario utilizar os métodos dispostos pelo Agente Path Component, pois o Agente irá utilizar o Path AStar por dentro de seuj código.

3. SplinePath

SplinePath, como o nome sugere, ele cria um spline pelo menor caminho encontrado, para assim ser utilizado para que seu personagem caminhe por dentro deste spline, todo momento de novo caminho, ele cria um novo Spline Path no mundo, e deleta o anterior, caso exista.

### Custom PathFinding (Main)

Aqui é seu projeto principal, por este motivo, tudo disposto aqui seria simulando oq um usuario poderia fazer para usar o sistema de PathFinding desenvolvido, dentro deste Main, contemos o nosso "MainPlayer", onde fica responsável em conter o Agent para o PAth e tbm é intermediário para realizar a conexão entre o grid e o Path Finding (Astar)

O player contem a funcionalidade de movimentar o personagem dentro do grid, clicando na area estabelicida para o grid
Consegue juntamente movimentar a camera no eixo Yaw em 90 graus
e contem um rayCast imbutido nele para realizar os inputs que utilizaram os outros modulos para "triggar" o pathFinding
Além de conter modo Debug para visualizar o algoritmo encontrando o caminho e o grid

## Debug

Para ativar o modo debug, basta ir no "PlayerController" já criado, e ativar a boolean chamada "isDebugMode", feito isso, poderá visualizar o grid no viewport e a escolha de caminho dentro da exedução

Além disto, cada módulo e script c++, tem uma variavel para definir e ativar o modo ""debug" da mesma, podendo conter estes debugs em outros projetos ou ideias, se assim for necessario
