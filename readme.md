Config file structure:
	- model: VGG16
	- layers (CONV 2D | GlobalAveragePooling):
		- 1
			- number of neurons e.g. 64
			- filter size e.g. (3, 3)
			- padding e.g. "same"
			- activation e.g. "relu"

What do I want to be able to do from this UI:
	- start the learning
		- this option should create a process running the python script that reads the training config from a config file...
	
	- create new network parameters
		- create a whole new entry into the config file

	- configure the network parameters (for an existing network configuration)
		- this one should be able to create a new layer
		- delete the layer
		- set the number of neurons
		- select the activation funcion
		- select the batch size
		- select the number of epochs

	- see the results of the training...

dev notes:
	- a menu is a new screen that is going to be drawn...
	- a function is supposed to be sth to be executed in the background...s



The config file has to have:
	- unique network configuration identifier
	- parameter ranges 
		- if for example one layer can have a range of neurons inside of it...
	- number of layers
	- configuration for each layer