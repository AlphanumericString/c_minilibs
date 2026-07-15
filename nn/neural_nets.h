#ifndef C_MINI__NEURAL_NETS_H
#define C_MINI__NEURAL_NETS_H

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
	t_neuron *layers;
} t_nn;

// de-serialize
t_nn     *nn_from_disk(int fd);
// serialize
void      nn_to_disk(t_nn *nn, int fd);

// eg for neural net of 3 layer with layer 0 of 32 layer 1 of 256, layer 3 of 32
// you would put (3, {32, 64, 32})
t_nn     *nn_create(size_t nb_layers, size_t *layer_sizes);
t_neuron *nn_run(t_neuron *input);
t_neuron *nn_train(t_neuron *input, t_neuron *expected);

#endif // !C_MINI__NEURAL_NETS_H
