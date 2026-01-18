# SS GitPush

Uma ferramenta inteligente de automatização de commits Git escrita em C++98 que gera mensagens de commit baseadas em comentários especiais no teu código.

## 📋 Descrição

O `ss_git_push` é um utilitário de linha de comandos que automatiza o processo de commit no Git através de:
- Preparação automática de todas as alterações (`git add *`)
- Análise de ficheiros modificados à procura de comentários especiais `SS_COMMIT`
- Geração de mensagens de commit estruturadas baseadas nesses comentários
- Criação de commits com descrições detalhadas ficheiro a ficheiro
- Push automático para o repositório remoto (`git push`)

## 🚀 Funcionalidades

- **Preparação Automática**: Prepara todas as alterações no repositório
- **Detecção Inteligente de Comentários**: Procura marcadores `// SS_COMMIT:` ou `#// SS_COMMIT:` no teu código
- **Mensagens de Commit Estruturadas**: Gera mensagens de commit organizadas com nomes de ficheiros e descrições de alterações
- **Suporte para Múltiplos Ficheiros**: Processa múltiplos ficheiros modificados num único commit
- **Remoção Opcional de Comentários**: Flag `-rm` para remover os comentários `SS_COMMIT` após o commit
- **Push Automático**: Envia automaticamente as alterações para o repositório remoto
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
│   ├── build_commit_message.cpp     # Gera mensagens de commit
│   ├── extract_commits_from_file.cpp # Extrai comentários SS_COMMIT
│   ├── get_staged_files.cpp         # Obtém lista de ficheiros preparados
│   ├── main.cpp                     # Lógica principal do programa
│   └── remove_commit_lines.cpp      # Remove linhas SS_COMMIT (com -rm)
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
1. Executar `git add *` (preparar todas as alterações)
2. Obter a lista de ficheiros preparados
3. Analisar cada ficheiro à procura de comentários `SS_COMMIT`
4. Gerar uma mensagem de commit estruturada
5. Criar o commit com a mensagem gerada
6. Executar `git push` para enviar as alterações

### Utilização com Flag `-rm`

```bash
./ss_git_push -rm
```

Com a flag `-rm`, o programa irá:
1. Executar todo o processo normal de commit e push
2. **Remover todas as linhas** que contêm os marcadores `SS_COMMIT` dos ficheiros
3. Se uma linha contiver apenas espaços/tabs seguidos do marcador, a linha inteira é removida
4. Se uma linha contiver código antes do marcador, apenas o marcador e o texto após ele são removidos

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

**Código na mesma linha (será preservado sem o marcador com `-rm`):**
```cpp
int x = 42;  // SS_COMMIT: Inicializada variável x
```

### Fluxo de Trabalho de Exemplo

```bash
# 1. Edita os teus ficheiros e adiciona comentários SS_COMMIT
vim src/main.cpp
# Adiciona: // SS_COMMIT: Implementada nova funcionalidade X

vim src/utils.cpp
# Adiciona: // SS_COMMIT: Refactoradas funções auxiliares

# 2. Executa ss_git_push
./ss_git_push

# 3. Ou executa com -rm para limpar os comentários após o commit
./ss_git_push -rm
```

### Exemplo de Mensagem de Commit Gerada

```
UPDATED FILE:

 - src/main.cpp:
   • Implementada nova funcionalidade X
   • Corrigido memory leak na inicialização

 - src/utils.cpp:
   • Refactoradas funções auxiliares

 - src/config.h
```

Nota: Ficheiros sem comentários `SS_COMMIT` aparecem listados sem marcadores de tópico.

## 📝 Sintaxe dos Comentários

A ferramenta reconhece dois formatos de comentários:

1. **Estilo C/C++**: `// SS_COMMIT: A tua mensagem aqui`
2. **Estilo Script**: `#// SS_COMMIT: A tua mensagem aqui`

**Regras:**
- Os comentários devem começar com `// SS_COMMIT:` ou `#// SS_COMMIT:`
- O texto após os dois pontos será usado como descrição da alteração
- Espaços em branco à esquerda são automaticamente removidos
- Múltiplos comentários no mesmo ficheiro serão todos incluídos
- Caracteres nulos (`\0`) são automaticamente removidos das mensagens

## 🎯 Casos de Uso

- **Fluxo de Desenvolvimento**: Faz commits rapidamente com mensagens descritivas
- **Revisão de Código**: Documenta alterações directamente no código
- **Colaboração em Equipa**: Garante formatação consistente das mensagens de commit
- **Projectos de Aprendizagem**: Acompanha alterações incrementais com descrições detalhadas
- **Limpeza de Código**: Usa `-rm` para remover comentários temporários após documentar alterações

## ⚠️ Notas Importantes

- A ferramenta executa automaticamente `git add *` (prepara todas as alterações)
- Se nenhum ficheiro estiver preparado, será apresentado "Nothing to commit!"
- Ficheiros sem comentários `SS_COMMIT` continuarão a ser listados no commit
- As mensagens de commit são geradas automaticamente
- O push é executado automaticamente após o commit
- Com `-rm`, os comentários são removidos dos ficheiros **após** o commit ser criado
- A flag `-rm` preserva código que aparece antes dos marcadores `SS_COMMIT`

## 📋 Argumentos da Linha de Comandos

```
Utilização: ./ss_git_push [-rm]

Opções:
  (nenhuma)    Executa commit e push normalmente, mantém comentários SS_COMMIT
  -rm          Executa commit e push, depois remove todas as linhas com SS_COMMIT
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
UPDATED FILE:

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
- Revê os commits gerados antes de fazer push para o repositório remoto
- Combina com Git hooks para automatização adicional
- Considera adicionar múltiplos comentários `SS_COMMIT` para alterações complexas
- Usa `-rm` quando os comentários são apenas temporários e não devem permanecer no código
- Evita usar `-rm` se quiseres manter um histórico de alterações nos comentários do código

## 🐛 Gestão de Erros

O programa trata os seguintes erros:

- **Demasiados argumentos**: Aceita apenas 0 ou 1 argumento
- **Argumento inválido**: Apenas `-rm` é aceite como argumento
- **Nada para commit**: Avisa se não existem ficheiros preparados
- **Erro ao criar ficheiro temporário**: Verifica se consegue criar o ficheiro de mensagem
- **Erro ao abrir ficheiro temporário**: Verifica se consegue escrever a mensagem

---

**Feito usando C++98**
