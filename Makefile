ifeq ($(OS),Windows_NT)
    FRONTEND_BIN = ./game.exe
    BACKEND_BIN = ./main.exe
else
    FRONTEND_BIN = ./game
    BACKEND_BIN = ./main
endif

.PHONY: dev frontend-make-run backend-make-run

dev:
	@$(MAKE) -j 2 frontend-make-run backend-make-run

frontend-make-run:
	$(MAKE) -C frontend
	cd frontend && $(FRONTEND_BIN)

backend-make-run:
	$(MAKE) -C backend
	cd backend && $(BACKEND_BIN)