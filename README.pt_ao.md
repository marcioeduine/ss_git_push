# SS GitPush

Uma ferramenta inteligente de automatização de commits Git escrita em C++98 que gera mensagens de commit baseadas em comentários especiais no teu código.

## 📋 Descrição

O `ss_git_push` é um utilitário de linha de comandos que automatiza o processo de commit no Git através de:
- Preparação automática de todas as alterações (`git add -A`, incluindo ficheiros novos, modificações e eliminações)
- Análise de ficheiros modificados à procura de comentários especiais `SS_COMMIT`
- Geração de mensagens de commit por estado (ficheiros novos, actualizados e removidos)
- Apresentação do ramo actual e da mensagem gerada antes do commit
- Criação de commits com descrições detalhadas ficheiro a ficheiro
- Push para o repositório remoto (`git push`), salvo indicação contrária
- Modo de simulação (`--dry-run`) que nada altera

## 🚀 Funcionalidades

- **Preparação Completa**: Prepara tudo com `git add -A`, incluindo dotfiles e eliminações
- **Detecção Inteligente de Comentários**: Procura marcadores `// SS_COMMIT:`, `#// SS_COMMIT:`, `{/* SS_COMMIT:` (JSX) e `/* SS_COMMIT:` (CSS/bloco) no teu código
- **Mensagens por Estado**: Agrupa os ficheiros em secções `NEW FILES`, `UPDATED FILES` e `REMOVED FILES` em vez de um cabeçalho único e fixo
- **Apresentação do Ramo**: Mostra sempre o ramo actual com a mensagem gerada
- **Modo de Simulação**: `--dry-run` mostra o que seria commitado sem tocar no índice, no histórico ou no remoto
- **Push Opcional**: `--no-push` faz commit sem push, para reveres depois
- **Suporte para Múltiplos Ficheiros**: Processa múltiplos ficheiros modificados num único commit
- **Remoção Opcional de Comentários**: Flag `-rm` remove os comentários `SS_COMMIT` após o push e commita a limpeza, de modo que a árvore termina limpa
- **Execução Verificada**: Falhas de preparação, commit ou push abortam a execução com erro claro; um push falhado nunca dispara a remoção de marcadores
- **Compatível com C++98**: Escrito em C++98 padrão para máxima compatibilidade

## 📧 Instalação

### Pré-requisitos
- Git instalado e configurado
- Compilador C++ com suporte para C++98 (g++, clang++)
- Make

### Compilar a partir do Código Fonte

```bash
# Clona o repositório
git clone <url-do-repositório>
cd ss_git_push

# Compila o projecto
make

# O binário será criado no directório actual
```

### Targets do Makefile

```bash
make        # Compila o projecto
make clean  # Remove ficheiros objecto
make fclean # Remove ficheiros objecto e binário
make re     # Recompila o projecto do zero
```

### Opcional: Adicionar ao PATH

```bash
# Podes precisar de conceder permissões ao binário
chmod +x ss_git_push

# Copia para um directório no teu PATH
sudo cp ss_git_push /usr/local/bin/

# Ou adiciona um alias à configuração da tua shell
echo 'alias ss_git_push="/caminho/para/ss_git_push"' >> ~/.bashrc
```

## 📁 Estrutura do Projecto

```
.
├── include/
│   └── ss_git_push.hpp              # Ficheiro de cabeçalho com declarações
├── src/
│   ├── build_commit_message.cpp     # Gera mensagens de commit por estado
│   ├── extract_commits_from_file.cpp # Extrai comentários SS_COMMIT
│   ├── get_staged_files.cpp         # Obtém ficheiros, estados e ramo actual
│   ├── main.cpp                     # Lógica principal e leitura de opções
│   ├── remove_commit_lines.cpp      # Remove linhas SS_COMMIT (com -rm)
│   └── run_command.cpp              # Execução verificada de comandos
├── Makefile                          # Configuração de compilação
├── README.md                         # Documentação (Inglês)
└── README.pt_ao.md                   # Documentação (Português)
```

## 📖 Utilização

### Utilização Básica

```bash
./ss_git_push
```

Este comando irá:
1. Executar `git add -A` (prepara tudo, incluindo eliminações e dotfiles)
2. Obter a lista de ficheiros preparados com o seu estado (novo / actualizado / removido)
3. Analisar cada ficheiro à procura de comentários `SS_COMMIT`
4. Apresentar o ramo actual e a mensagem gerada
5. Criar o commit com a mensagem gerada
6. Executar `git push` para enviar as alterações

### Utilização com Flag `-rm`

```bash
./ss_git_push -rm
```

Com a flag `-rm`, o programa irá:
1. Executar todo o processo normal de commit e push
2. **Remover todas as linhas** que contêm os marcadores `SS_COMMIT` dos ficheiros commitados
3. Se uma linha contiver apenas espaços/tabs seguidos do marcador, a linha inteira é removida
4. Se uma linha contiver código antes do marcador, apenas o marcador e o texto após ele são removidos
5. Preparar a limpeza e criar um segundo commit (`chore: remove SS_COMMIT markers`)
6. Fazer push da limpeza, de modo que a árvore termina limpa

### Utilização com `--no-push`

```bash
./ss_git_push --no-push
# ou: ./ss_git_push -n
```

Faz commit normalmente mas salta ambos os pushes. Útil quando queres rever o
commit em local primeiro, ou agrupar vários commits antes do push. Combina com
`-rm` (o commit de limpeza também fica em local).

### Utilização com `--dry-run`

```bash
./ss_git_push --dry-run
# ou: ./ss_git_push -d
```

Modo de simulação. Lê a árvore de trabalho sem preparar nada e apresenta o
ramo mais a mensagem que seria gerada. Nada é preparado, commitado ou
empurrado. Não pode ser combinado com `-rm`.

### Utilização com `--help`

```bash
./ss_git_push --help
# ou: ./ss_git_push -h
```

Apresenta o resumo de utilização.

### Adicionar Comentários SS_COMMIT

Adiciona comentários especiais nos teus ficheiros modificados para descrever as alterações:

**Para ficheiros C/C++:**
```cpp
// SS_COMMIT: Adicionada função de autenticação de utilizador
void	authenticate_user(void)
{
    // implementação
}
```

**Para scripts Python/Shell:**
```python
#// SS_COMMIT: Corrigido bug na validação de dados
def	validate_data(input):
    # implementação
```

**Para componentes JSX/React (válido dentro do markup):**
```jsx
{/* SS_COMMIT: Alinhados ícones do cabeçalho */}
<button>Play</button>
```

**Para CSS/folhas de estilo:**
```css
/* SS_COMMIT: Centrado rodapé da vitrina */
.footer { display: flex; }
```

**Código na mesma linha (será preservado sem o marcador com `-rm`):**
```cpp
int x = 42;  // SS_COMMIT: Inicializada variável x
```

### Fluxo de Trabalho de Exemplo

```bash
# 1. Edita os teus ficheiros e adiciona comentários SS_COMMIT
vim src/main.cpp
# Adiciona: // SS_COMMIT: Implementada nova funcionalidade X

vim src/App.jsx
# Adiciona: {/* SS_COMMIT: Alinhados ícones do cabeçalho */}

# 2. Previsualiza com simulação
./ss_git_push --dry-run

# 3. Commit e push
./ss_git_push

# 4. Ou executa com -rm para limpar os comentários após o commit
./ss_git_push -rm
```

### Exemplo de Mensagem de Commit Gerada

```
NEW FILES:
 - src/App.jsx:
   • Aligned header icons

UPDATED FILES:
 - src/main.cpp:
   • Implementada nova funcionalidade X
   • Corrigido memory leak na inicialização

REMOVED FILES:
 - src/legacy.cpp
```

Nota: Ficheiros sem comentários `SS_COMMIT` aparecem listados sem marcadores de tópico.
Secções vazias são omitidas.

## 📝 Sintaxe dos Comentários

A ferramenta reconhece quatro formatos de comentários:

1. **Estilo C/C++**: `// SS_COMMIT: A tua mensagem aqui`
2. **Estilo Script**: `#// SS_COMMIT: A tua mensagem aqui`
3. **Estilo JSX**: `{/* SS_COMMIT: A tua mensagem aqui */}`
4. **Estilo Bloco**: `/* SS_COMMIT: A tua mensagem aqui */` (CSS e comentários de bloco)

**Regras:**
- A mensagem termina no primeiro `*/` da linha, de modo que os fechos nunca
  contaminam o texto do commit
- Fechos `*/` e `}` no fim são removidos automaticamente
- O texto após os dois pontos será usado como descrição da alteração
- Espaços em branco à esquerda são automaticamente removidos
- Múltiplos comentários no mesmo ficheiro serão todos incluídos
- Caracteres nulos (`\0`) são automaticamente removidos das mensagens
- Dentro do markup JSX, usa a forma `{/* ... */}`: um marcador `//` aí é erro
  de sintaxe em React

## 🎯 Casos de Uso

- **Fluxo de Desenvolvimento**: Faz commits rapidamente com mensagens descritivas
- **Revisão de Código**: Documenta alterações directamente no código
- **Colaboração em Equipa**: Garante formatação consistente das mensagens de commit
- **Projectos de Aprendizagem**: Acompanha alterações incrementais com descrições detalhadas
- **Limpeza de Código**: Usa `-rm` para remover comentários temporários após documentar alterações

## ⚠️ Notas Importantes

- A ferramenta executa `git add -A` (prepara tudo, incluindo eliminações e dotfiles)
- O ramo actual é sempre apresentado com a mensagem gerada: revê com
  `--dry-run` antes do push, sobretudo no `main`
- Se nenhum ficheiro estiver preparado, será apresentado "Nothing to commit!"
- Ficheiros sem comentários `SS_COMMIT` continuarão a ser listados no commit
- O push é executado automaticamente após o commit, salvo `--no-push`
- Com `-rm`, a limpeza dos marcadores é commitada (`chore: remove SS_COMMIT markers`)
  e empurrada, de modo que a árvore termina limpa
- Se a preparação, o commit ou o push falharem, a execução aborta com erro e os
  marcadores são mantidos: um push falhado nunca dispara a remoção
- Caminhos com espaços ou citação especial não são suportados

## 📋 Argumentos da Linha de Comandos

```
Utilização: ./ss_git_push [-rm] [-n|--no-push] [-d|--dry-run]

Opções:
  (nenhuma)    Prepara, commit e push; mantém comentários SS_COMMIT
  -rm          Commit, push, depois remove linhas SS_COMMIT e commita a limpeza
  -n, --no-push  Faz commit sem push
  -d, --dry-run  Mostra ramo e mensagem gerada; nada altera
  -h, --help   Mostra ajuda de utilização
```

## 🔍 Exemplo Completo

**Ficheiro: main.cpp (antes)**
```cpp
#include <iostream>

// SS_COMMIT: Adicionada função hello world
void	hello(void)
{
    std::cout << "Hello, World!" << std::endl;
}

// SS_COMMIT: Actualizado main para usar nova função hello
int	main(void)
{
    return (hello(), 0);
}

int x = 42;  // SS_COMMIT: Inicializada variável global
```

**Executar ss_git_push com -rm:**
```bash
$ ./ss_git_push -rm
```

**Commit gerado:**
```
UPDATED FILES:
 - main.cpp:
   • Adicionada função hello world
   • Actualizado main para usar nova função hello
   • Inicializada variável global
```

**Ficheiro: main.cpp (depois do -rm)**
```cpp
#include <iostream>

void	hello(void)
{
    std::cout << "Hello, World!" << std::endl;
}

int	main(void)
{
    return (hello(), 0);
}

int x = 42;
```

## 🛠️ Detalhes Técnicos

- **Linguagem**: C++98
- **Dependências**: Biblioteca padrão C++, chamadas de sistema POSIX
- **Compatibilidade**: Linux, macOS, sistemas Unix-like
- **Chamadas de sistema utilizadas**: `system()`, `popen()`, `pclose()`, `mkstemp()`, `remove()`
- **Comandos Git utilizados**: `git add -A`, `git diff --name-only --cached`, `git status --porcelain`, `git rev-parse --abbrev-ref HEAD`
- **Gestão de Ficheiros Temporários**: Cria ficheiro temporário em `/tmp/` para a mensagem de commit
- **Processamento de Texto**: Remove automaticamente caracteres nulos e espaços em branco desnecessários
- **Flags de Compilação**: `-Wall -Wextra -Werror -std=c++98`

## 📄 Licença

Este projecto é open source e está disponível para uso pessoal e comercial.

SS é apenas uma assinatura e significa Ser Superior em português. Todos os meus projectos têm este prefixo.

## 🤝 Contribuir

Sente-te à vontade para fazer fork, modificar e submeter pull requests. Sugestões e melhorias são bem-vindas!

## 💡 Dicas

- Usa comentários `SS_COMMIT` descritivos para um melhor histórico de commits
- Previsualiza com `--dry-run` antes do push para o repositório remoto
- Combina com Git hooks para automatização adicional
- Considera adicionar múltiplos comentários `SS_COMMIT` para alterações complexas
- Usa `-rm` quando os comentários são apenas temporários e não devem permanecer no código
- Evita usar `-rm` se quiseres manter um histórico de alterações nos comentários do código

## 🐛 Gestão de Erros

O programa trata os seguintes erros:

- **Demasiados argumentos**: Aceita apenas combinações documentadas de flags
- **Argumento inválido**: Apenas `-rm`, `-n`/`--no-push`, `-d`/`--dry-run`, `-h`/`--help` são aceites
- **Flags em conflito**: `-rm` e `--dry-run` não podem ser combinados
- **Nada para commit**: Avisa se não existem ficheiros preparados (ou alterados, na simulação)
- **Falhas de preparação/commit/push**: Aborta com erro claro; marcadores mantidos
- **Erro ao criar ficheiro temporário**: Verifica se consegue criar o ficheiro de mensagem
- **Erro ao abrir ficheiro temporário**: Verifica se consegue escrever a mensagem

---

**Feito usando C++98**
