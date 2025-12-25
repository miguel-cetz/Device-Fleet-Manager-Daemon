# Variables for routes
PROTOC = /home/mike/gRPC/bin/protoc
GRPC_PLUGIN = /home/mike/gRPC/bin/grpc_cpp_plugin
PROTO_FILE = DeviceFleetManager.proto

# Files we expect to be generated
GENERATED_FILES = DeviceFleetManager.pb.cc DeviceFleetManager.pb.h \
                  DeviceFleetManager.grpc.pb.cc DeviceFleetManager.grpc.pb.h

# Default rule: compile the proto
all: $(GENERATED_FILES)

# Build rule: Only run if DeviceFleetManager.proto changed
$(GENERATED_FILES): $(PROTO_FILE)
	@echo "Detectado cambio en $(PROTO_FILE). Generando archivos..."
	$(PROTOC) --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_PLUGIN) $(PROTO_FILE)
	@echo "¡Archivos generados con éxito!"

# Clean generated files
clean:
	rm -f $(GENERATED_FILES)

.PHONY: all clean
