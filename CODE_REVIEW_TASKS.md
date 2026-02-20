# Revisão da base de código: problemas encontrados e tarefas sugeridas

## 1) Tarefa para corrigir erro de digitação
**Problema encontrado:** há erros de digitação em comentários, como `histrogama` e `Calculadno`, que prejudicam legibilidade e profissionalismo do código.

**Evidências:**
- `//começa o histrogama setado em 0`
- `// Calculadno a Frequencia acumulada`

**Tarefa sugerida:**
- Padronizar e corrigir ortografia dos comentários em `unit1.pas` (ex.: `histograma`, `Calculando a frequência acumulada`) e revisar comentários adjacentes para manter consistência.

## 2) Tarefa para corrigir bug
**Problema encontrado:** `newName` usa buffer fixo de 256 bytes e concatena sufixo com `snprintf` sem validar adequadamente o tamanho do nome de entrada; nomes longos podem gerar truncamento silencioso, e o fluxo não valida falha de `malloc` em todos os chamadores.

**Evidências:**
- Alocação fixa: `malloc(256 * sizeof(char))`
- Construção de string dependente de tamanho do input: `snprintf(aux + i, 256 - i, "_%s.bmp", addName);`

**Tarefa sugerida:**
- Refatorar `newName` para calcular tamanho dinâmico com base em `strlen(fileName)` e `strlen(addName)`, retornar erro explícito em caso de falha, e fazer os chamadores (`converterCinza`, `negativarImagem`) abortarem com limpeza correta quando o retorno for `NULL`.

## 3) Tarefa para ajustar comentário/discrepância de documentação
**Problema encontrado:** no fluxo de compressão dinâmica, o texto exibido ao usuário menciona `valor de y (gama)`, mas a variável usada no cálculo é `gamma`; isso gera inconsistência terminológica entre interface e implementação.

**Evidências:**
- Prompt: `Digite o valor de y (gama) (Maior que 0):`
- Código: `gamma := StrToFloat(gStr);` e `Power(rf, gamma)`

**Tarefa sugerida:**
- Atualizar texto de prompt/comentários para usar o mesmo termo da implementação (ex.: `gamma` ou `γ`) e, opcionalmente, incluir breve descrição da fórmula no comentário do método para reduzir ambiguidade.

## 4) Tarefa para melhorar um teste
**Problema encontrado:** não há testes automatizados garantindo comportamento estável das rotinas de processamento de BMP (`copiarImagem`, `converterCinza`, `negativarImagem`).

**Evidências:**
- Projeto C sem diretório de testes e sem comandos de teste versionados.

**Tarefa sugerida:**
- Criar suíte mínima de regressão para o binário C usando imagem fixture pequena (ex.: 2x2 ou 4x4), validando:
  1. preservação de cabeçalho BMP após cópia;
  2. valores esperados de pixels na conversão para cinza;
  3. inversão correta de canais no negativo;
  4. tamanho de saída idêntico ao de entrada para formatos suportados (24/32 bpp).
