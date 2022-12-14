INCLUDE_DIR = include
SRC_DIR     = src
BUILD_DIR   = build
TARGET      = taas-fargo

CXX       = clang++
CFLAGS    = -Wall -Wno-parentheses -Wno-sign-compare -Wno-writable-strings -std=c++11 -O3 -stdlib=libc++
LFLAGS    = -Wall
IFLAGS    = -I include

$(TARGET): $(SRC_DIR)/taas_fargo.cpp $(BUILD_DIR)/taas_af.o $(BUILD_DIR)/taas_solver.o $(BUILD_DIR)/taas_problem.o $(BUILD_DIR)/taas_labeling.o $(BUILD_DIR)/taas_fast_pqueue.o $(BUILD_DIR)/taas_compare.o $(BUILD_DIR)/taas_util.o
	@echo "Building and linking" $(TARGET)
	@$(CXX) $(CFLAGS) $(IFLAGS) $(SRC_DIR)/taas_fargo.cpp -o $(TARGET) $(BUILD_DIR)/taas_af.o $(BUILD_DIR)/taas_solver.o $(BUILD_DIR)/taas_problem.o $(BUILD_DIR)/taas_labeling.o $(BUILD_DIR)/taas_compare.o $(BUILD_DIR)/taas_util.o $(BUILD_DIR)/taas_fast_pqueue.o $(LFLAGS)

$(BUILD_DIR)/taas_af.o: $(INCLUDE_DIR)/taas_af.h $(SRC_DIR)/taas_af.cpp
	@echo "Building taas::Af"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_af.cpp -o $@

$(BUILD_DIR)/taas_problem.o: $(INCLUDE_DIR)/taas_problem.h $(SRC_DIR)/taas_problem.cpp
	@echo "Building taas::Problem"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_problem.cpp -o $@

$(BUILD_DIR)/taas_solver.o: $(INCLUDE_DIR)/taas_solver.h $(SRC_DIR)/taas_solver.cpp $(BUILD_DIR)/taas_problem.o $(BUILD_DIR)/taas_labeling.o
	@echo "Building taas::Solver"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_solver.cpp -o $@

$(BUILD_DIR)/taas_labeling.o: $(INCLUDE_DIR)/taas_labeling.h $(SRC_DIR)/taas_labeling.cpp $(BUILD_DIR)/taas_af.o
	@echo "Building taas::Labeling"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_labeling.cpp -o $@

$(BUILD_DIR)/taas_compare.o: $(INCLUDE_DIR)/taas_compare.h $(SRC_DIR)/taas_compare.cpp
	@echo "Building taas::ArgumentCompare"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_compare.cpp -o $@

$(BUILD_DIR)/taas_util.o: $(INCLUDE_DIR)/taas_util.h $(SRC_DIR)/taas_util.cpp
	@echo "Building taas::Util"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_util.cpp -o $@

$(BUILD_DIR)/taas_fast_pqueue.o: $(INCLUDE_DIR)/taas_fast_pqueue.h $(SRC_DIR)/taas_fast_pqueue.cpp
	@echo "Building taas::FastPriorityQueue"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/taas_fast_pqueue.cpp -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)/* $(TARGET)
