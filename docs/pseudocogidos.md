# Pseudocódigos

A seguir são apresentados os pseudocódigos das duas operações centrais do sistema, coerentes com os fluxogramas apresentados:

* **Cadastro de uma ação**
* **Geração do resumo agregado**

---

## 1. Cadastrar Ação

```text
INÍCIO cadastrarAcao
  LER escola, subtema, dataPrevista, publicoAlvo, responsavel, quantidadePrevista

  SE algum campo obrigatório estiver vazio ENTÃO
    EXIBIR "erro: preencha todos os campos obrigatórios"
    RETORNAR
  FIM SE

  SE quantidadePrevista < 0 ENTÃO
    EXIBIR "erro: quantidade não pode ser negativa"
    RETORNAR
  FIM SE

  GERAR novoCodigo

  PARA CADA acao JÁ CADASTRADA FAÇA
    SE acao.codigo = novoCodigo ENTÃO
      EXIBIR "erro: código já existe"
      RETORNAR
    FIM SE
  FIM PARA

  situacao ← "planejada"

  INSERIR nova ação no vetor de ações

  EXIBIR "ação cadastrada com sucesso, código:", novoCodigo
FIM cadastrarAcao
```

---

## 2. Gerar Resumo

```text
INÍCIO gerarResumo
  SE total de ações cadastradas = 0 ENTÃO
    EXIBIR "nenhuma ação cadastrada"
    RETORNAR
  FIM SE

  planejadas ← 0
  realizadas ← 0
  canceladas ← 0
  totalParticipantes ← 0
  totalPrevisto ← 0

  PARA CADA acao NO vetor de ações FAÇA

    SE acao.situacao = "planejada" ENTÃO
      planejadas ← planejadas + 1

    SENÃO SE acao.situacao = "realizada" ENTÃO
      realizadas ← realizadas + 1
      totalParticipantes ← totalParticipantes + acao.quantidadeRealizada
      ACUMULAR estresseMedioRelatado por subtema e por escola

    SENÃO SE acao.situacao = "cancelada" ENTÃO
      canceladas ← canceladas + 1
    FIM SE

    totalPrevisto ← totalPrevisto + acao.quantidadePrevista

    ACUMULAR contagem de ações por subtema e por escola

  FIM PARA

  percentualParticipacao ← (totalParticipantes / totalPrevisto) * 100

  EXIBIR planejadas, realizadas, canceladas, totalParticipantes, percentualParticipacao

  EXIBIR resumo agrupado por subtema
  EXIBIR resumo agrupado por escola

FIM gerarResumo
```

---

### 📌 Observação

Os pseudocódigos representam as operações centrais definidas para o sistema e servem como base para a implementação das funcionalidades em linguagem **C**.
