
#include <nn/neural_nets.h>
#include <stddef.h>
#include <stdlib.h>

t_nn *nn_create(size_t nb_layers, size_t *layer_sizes) {
	t_nn *nn;

	nn = malloc(sizeof(t_nn));
	return (nn_init(nn, nb_layers, layer_sizes));
}
t_nn *nn_init(t_nn *to_init, size_t nb_layers, size_t *layer_sizes) {
	size_t sum;
	char  *mem;

	if (!to_init) return NULL;
	to_init->nb_layers = nb_layers;
	sum                = 0;
	for (; nb_layers; sum += layer_sizes[--nb_layers])
		;
	mem = malloc(sizeof(t_layers) * nb_layers + sizeof(t_neuron) * sum);
	to_init->layers = (t_layers *)mem;
	mem            += sizeof(t_layers) * nb_layers;
	for (size_t i = 0; i < to_init->nb_layers; i++) {
		to_init->layers[i].nb_neurons = layer_sizes[i];
		to_init->layers[i].bias       = NULL;
		to_init->layers[i].weights    = (t_neuron *)((char *)mem);
		mem                          += sizeof(t_neuron *) * layer_sizes[i];
	}
	return (to_init);
}
void nn_clear(t_nn *nn) {
	if (!nn) return;
	for (size_t i = 0; i < nn->nb_layers; i++)
		for (size_t j = 0; j < nn->layers[i].nb_neurons; j++) {
			nn->layers[i].bias[j]    = 0;
			nn->layers[i].weights[j] = 0;
		}
}
void nn_delete(t_nn *nn) {
	if (!nn) return;
	if (nn->layers) free(nn->layers);
	nn->layers    = NULL;
	nn->nb_layers = 0;
}
void nn_free(t_nn *nn) {
	if (!nn) return;
	nn_delete(nn);
	free(nn);
}
