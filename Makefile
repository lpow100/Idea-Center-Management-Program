ifeq ($(OS),Windows_NT)
    FRONTEND_BIN = ./game.exe
    BACKEND_BIN = ./main.exe
else
    FRONTEND_BIN = ./game
    BACKEND_BIN = ./main
endif

.PHONY: dev frontend-make-run backend-make-run

dev:
	@$(MAKE) -j 2 frontend-make backend-make
	@$(MAKE) -j 2 frontend-run backend-run

frontend-make:
	$(MAKE) -C frontend
frontend-run:
	cd frontend && $(FRONTEND_BIN)

backend-make:
	$(MAKE) -C backend
backend-run:
	cd backend && $(BACKEND_BIN)