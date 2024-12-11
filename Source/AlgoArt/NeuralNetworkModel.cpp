// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetworkModel.h"

// Initialize the Neural Network Model
void UNeuralNetworkModel::InitModel()
{
	// Clear any previous weights and bias
	Weights.Empty();
	Bias.Empty();

	// Create the Hidden Layers Weights and Bias
	if (HiddenLayers.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Hidden Layers Array. Hidden Layers Array should not be empty."));
		return;
	}

	int PrevSize = InputsNumber;
	for (int k : HiddenLayers) {
		FMatrixNN weight = FMatrixNN(PrevSize, k);
		weight.RandomInit();

		FMatrixNN bias = FMatrixNN(1, k);
		bias.RandomInit();

		Weights.Push(weight);
		Bias.Push(bias);
		PrevSize = k;
	}

	// Create the Output Layer
	FMatrixNN weight = FMatrixNN(PrevSize, OutputsNumber);
	weight.RandomInit();

	FMatrixNN bias = FMatrixNN(1, OutputsNumber);
	bias.RandomInit();

	Weights.Push(weight);
	Bias.Push(bias);
}

// Run a Forward pass of the Neural Network Model and output the result
FMatrixNN UNeuralNetworkModel::RunForwardModel(FMatrixNN X)
{
	if (X.GetColumns() != Weights[0].GetRows()) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Input Size Missmatch"));
		return FMatrixNN(0, 0);
	}
	for (int k = 0; k < Weights.Num(); k++) {
		X = (X * Weights[k]) + Bias[k];
		X.ApplyTanH();
	}

	return X;
}

// Initialize the Number of Inputs
void UNeuralNetworkModel::SetInputsNumber(int number)
{
	InputsNumber = number;
}

// Initialize the Number of Outputs
void UNeuralNetworkModel::SetOutputsNumber(int number)
{
	OutputsNumber = number;
}

// Initialize the Config for Hidden Layers
void UNeuralNetworkModel::SetHiddenLayers(TArray<int>& hiddenlayers)
{
	HiddenLayers = hiddenlayers;
}