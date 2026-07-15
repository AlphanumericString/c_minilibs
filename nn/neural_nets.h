#ifndef C_MINI__NEURAL_NETS_H
#define C_MINI__NEURAL_NETS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Simple fixed-point neural network layer
typedef int16_t t_neuron; // Q7.8 format (adjust as needed)

typedef struct {
	size_t    nb_neurons;
	t_neuron *weights;
	t_neuron *bias;
} t_layers;

typedef struct {
	size_t    nb_layers;
	t_layers *layers;
} t_nn;

// de-serialize
bool nn_from_disk(t_nn *to_init, int fd);
// serialize
bool nn_to_disk(t_nn *nn, int fd);

// eg for neural net of 3 layer with layer 0 of 32 layer 1 of 256, layer 3 of 32
// you would put (3, {32, 64, 32})
t_nn *nn_create(size_t nb_layers, size_t *layer_sizes);
t_nn *nn_init(t_nn *to_init, size_t nb_layers, size_t *layer_sizes);
void  nn_clear(t_nn *nn);
void  nn_delete(t_nn *nn);
void  nn_free(t_nn *nn);

t_neuron *nn_run(t_neuron *input);
t_neuron *nn_train(t_neuron *input, t_neuron *expected);

#endif // !C_MINI__NEURAL_NETS_H
