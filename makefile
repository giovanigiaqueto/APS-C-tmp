
# pastas com dos códigos e de compilação
OBJDIR=obj
SRCDIR=src
INCDIR=inc

# parâmetros do compilador
CFLAGS=-Wall -std=c11 -I$(INCDIR)/

# modulos e executável principal
modulos=$(addprefix $(OBJDIR)/, main.o ler_arquivo.o ordenacao_bolha.o select_sort.o quick_sort.o)
alvo=main

# regra principal de compilação
all: $(alvo)

# regra de compilação do executável
$(alvo): $(modulos)
	@echo "linkando '$(notdir $(alvo))'"
	@$(CC) -o $(alvo) $(modulos) $(CFLAGS)

# regra de compilação dos modulos
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compilando '$(notdir $<)'"
	@$(CC) -c -o $@ $<

# adiciona regras de dependencia entre arquivos
include deps.mk

# regra de limpeza da pasta de compilação de modulos
limpar:
	@echo "removendo objetos de compilação"
	@$(RM) -f $(modulos)

