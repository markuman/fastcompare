build: ## compile fastcompare
	@echo "compiling fastcompare"
	@gcc fastcompare.c -lz -Ofast -pipe -march=native -mtune=native -o fastcompare

	
.PHONY: help

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.DEFAULT_GOAL := help

