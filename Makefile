CXX=clang++ -std=c++17
CXX_FLAG=-I includes -I model -O3 -Wno-undefined-inline
WASM_FLAG=-DWASM_DEMO -Wl,--no-entry, -Wl,--export=HEIGHT -Wl,--export=WIDTH, -Wl,--export=CANVAS, -Wl,--export=render -Wl,--allow-undefined -Wl,--import-memory --target=wasm32 -fno-builtin --no-standard-libraries 


OBJ:=objs
SRC:=src
TEST_ENTRY:=$(SRC)/test.cc
TERM_ENTRY:=$(SRC)/Terminal.cc
DEMO:=demo
WASM_OUT:=docs
BIN:=bin
TEST:=$(BIN)/test
TERM:=$(BIN)/term
SRC_COMMON:=$(SRC)/common
OBJ_COMMON:=$(OBJ)/common

COMMONS:=$(wildcard $(SRC_COMMON)/*.cc)
DEMOS:=$(wildcard $(DEMO)/*.cc)
WASMS:= $(patsubst $(DEMO)%, $(WASM_OUT)%, $(patsubst %.cc, %.wasm, $(DEMOS)))
OBJS:=$(patsubst $(DEMO)%, $(OBJ)%, $(patsubst %.cc, %.o, $(DEMOS)))
OBJS_COMMON:=$(patsubst $(SRC_COMMON)%, $(OBJ_COMMON)%, $(patsubst %.cc, %.o, $(COMMONS)))
TESTS:=$(patsubst $(DEMO)%, $(TEST)%, $(patsubst %.cc, %, $(DEMOS)))
TERMS:=$(patsubst $(DEMO)%, $(TERM)%, $(patsubst %.cc, %, $(DEMOS)))


obj: $(OBJS) obj-common
obj-common: $(OBJS_COMMON)
wasm: $(WASMS)
test: $(TESTS)
term: $(TERMS)

$(OBJ)/%.o: $(DEMO)/%.cc
	$(CXX) -c $(CXX_FLAG) $^  -o $@
$(OBJ_COMMON)/%.o: $(SRC_COMMON)/%.cc
	$(CXX) -c $(CXX_FLAG) $^  -o $@
$(WASM_OUT)/%.wasm: $(DEMO)/%.cc $(COMMONS)
	${CXX} ${CXX_FLAG} ${WASM_FLAG} $^ -o $@ 
$(TEST)/%: $(OBJ)/%.o ${OBJS_COMMON} $(TEST_ENTRY)
	$(CXX) $(CXX_FLAG) $^  -o $@
$(TERM)/%: $(OBJ)/%.o ${OBJS_COMMON} $(TERM_ENTRY)
	$(CXX) $(CXX_FLAG) $^ -o $@

.PHONY:= clean obj wasm test
.DEFAULT_GOAL:= wasm
clean:
	rm -f ${TEST}/*
	rm -f $(TERM)/*
	rm -f $(OBJ)/*.o
	rm -f $(OBJ_COMMON)/*.o
	rm -f ${WASM_OUT}/*.wasm

