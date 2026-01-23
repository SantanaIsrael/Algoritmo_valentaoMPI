**Descrição:** Implementação do Algoritmo do Valentão utilizando MPI (Message Passing Interface) para comunicação entre processos em um sistema distribuído.

# O que é agoritimo do valentão?
O algoritmo do valentão (ou "bully algorithm") é um protocolo de eleição de líder utilizado em sistemas distribuídos para garantir que um único processo seja escolhido como o coordenador ou líder entre vários processos concorrentes. Esse algoritmo é especialmente útil em ambientes onde os processos podem falhar ou se desconectar.

## Como funciona o algoritmo do valentão?

1. **Início da Eleição:** Quando um processo detecta que o líder atual falhou (por exemplo, não responde a mensagens de "heartbeat"), ele inicia uma eleição. O processo que inicia a eleição envia uma mensagem de eleição para todos os processos com IDs maiores do que o seu.
2. **Resposta dos Processos:** Os processos que recebem a mensagem de eleição respondem com uma mensagem de "OK" se estiverem ativos e tiverem um ID maior do que o processo que iniciou a eleição. Isso indica que eles estão dispostos a assumir o papel de líder.
3. **Escolha do Líder:** Se o processo que iniciou a eleição não receber nenhuma resposta "OK" de processos com IDs maiores, ele se declara o líder e envia uma mensagem de "vencedor" para todos os outros processos. Caso contrário, ele aguarda a eleição ser concluída por um dos processos com ID maior.
4. **Anúncio do Líder:** O processo que se torna o líder anuncia sua posição para todos os outros processos, que então reconhecem esse processo como o novo líder.

## Vantagens do Algoritmo do Valentão:
- Simplicidade: O algoritmo é relativamente simples de implementar e entender.

- Robustez: Pode lidar com falhas de processos, desde que haja pelo menos um processo ativo com o maior ID.

## Desvantagens do Algoritmo do Valentão:

- Sobrecarga de Mensagens: Em sistemas com muitos processos, o número de mensagens trocadas pode ser alto, o que pode levar a congestionamento na rede.

- Tempo de Eleição: O tempo necessário para concluir uma eleição pode ser significativo, especialmente em sistemas grandes.

- Dependência de IDs: O algoritmo depende da existência de IDs únicos e ordenados para os processos, o que pode não ser ideal em todos os cenários.

## Quando usar o Algoritmo do Valentão?

O algoritmo do valentão é adequado para sistemas distribuídos onde a eleição de um líder é necessária e onde os processos podem falhar. Ele é frequentemente utilizado em sistemas de banco de dados distribuídos, sistemas de arquivos distribuídos e outros ambientes onde a coordenação entre processos é crucial para o funcionamento correto do sistema.

# O que é MPI?
MPI (Message Passing Interface) é um padrão de comunicação utilizado em computação paralela para permitir que processos independentes troquem mensagens entre si. Ele é amplamente utilizado em ambientes de computação de alto desempenho (HPC) para desenvolver aplicações que podem ser executadas em múltiplos nós de um cluster ou supercomputador.

**Imagine o seguinte cenário:**

- Você tem 10 computadores ligados em rede.

- Cada computador tem seu próprio processador e sua própria memória RAM.

- O Computador A não consegue ler diretamente uma variável na memória do Computador B.

Para que o Computador A e o Computador B trabalhem juntos, o Computador A precisa enviar um pacote de dados (uma mensagem) pela rede, e o Computador B precisa explicitamente receber essa mensagem. O MPI padroniza como isso é feito.

### Como funciona na prática?
Quando você roda um programa com MPI, você lança múltiplas cópias do mesmo programa (chamadas de processos):

**Communicator (Comunicador):** É o grupo de todos os processos que podem conversar entre si. O comunicador padrão é chamado `MPI_COMM_WORLD`.

**Rank (Posto/ID):** Dentro do comunicador, cada processo recebe um número de identificação único, começando do 0.

- O processo com Rank 0 geralmente é o "mestre" (coordena o trabalho).

- Os processos com Rank 1, 2, 3... são os "trabalhadores".

#### Operações de Envio e Recebimento:

**Ponto-a-Ponto:** Um processo envia dados para outro processo específico (ex: `MPI_Send` e `MPI_Recv`).

**Coletivas:** Um processo envia dados para todos os outros, ou coleta dados de todos para um só (ex: `MPI_Bcast`, `MPI_Reduce`).

#### Tipos de MPI (Implementações comuns):

- **Open MPI:** Uma implementação de código aberto do padrão MPI, amplamente utilizada em ambientes acadêmicos e de pesquisa.
- **MPICH:** Outra implementação popular do MPI, conhecida por sua portabilidade e desempenho.
- **Mpi4py:**A biblioteca padrão para usar MPI com a linguagem Python.

#### Entendendo os argumentos `MPI_send` e `MPI_recv`:

- **buffer:** O local na memória onde os dados a serem enviados ou recebidos estão armazenados.
- **count:** O número de elementos a serem enviados ou recebidos.
- **datatype:** O tipo de dados dos elementos (ex: `MPI_INT`, `MPI_FLOAT`).
- **dest/source:** O rank do processo destino (para envio) ou origem (para recebimento).
    - No Send, é o Rank de quem recebe (Destino).
    - No Recv, é o Rank de quem enviou (Origem).
- **tag:** Um identificador para a mensagem, usado para diferenciar entre diferentes tipos de mensagens.
- **comm:** O comunicador que define o grupo de processos envolvidos na comunicação.

## Instalando o MPI:
Para instalar o MPI (Message Passing Interface) em seu sistema, você pode usar um gerenciador de pacotes. Como o exemplo foi desenvolvido em um ambiente Linux, utilizando o WSL, aqui estão os comandos para instalar o Open MPI ou MPICH, que são duas das implementações mais comuns do MPI.

Aqui estão os comandos para Ubutun/Debian:

- **Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install mpich
```
### Compilando um MPI Program:
Para compilar um programa escrito em C ou C++ que utiliza MPI, você deve usar o compilador MPI fornecido pela implementação que você instalou. Aqui está um exemplo de como compilar um programa MPI em C usando o `mpicc`, que é o compilador MPI para C.  

```bash
mpicc -o meu_programa_mpi meu_programa_mpi.c
```
### Executando um MPI:
Para executar um programa MPI, você geralmente usa o comando `mpirun` ou `mpiexec`, seguido pelo número de processos que deseja iniciar e o nome do executável do seu programa. Aqui está um exemplo básico:

```bash
mpirun -np 4 ./meu_programa_mpi
```
Para forçar a execução considerar os núcleos virtuais, você pode usar a opção `--use-hwthread-cpus`:

```bash
mpirun --use-hwthread-cpus -np 5 meu_programa_mpi
```
ou

```bash
mpirun --oversubscribe -np 5 meu_programa_mpi
```
