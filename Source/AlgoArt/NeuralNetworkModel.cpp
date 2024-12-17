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
		FMatrixNM weight = FMatrixNM(PrevSize, k);
		weight.RandomInit();

		FMatrixNM bias = FMatrixNM(1, k);
		bias.RandomInit();

		Weights.Push(weight);
		Bias.Push(bias);
		PrevSize = k;
	}

	// Create the Output Layer
	FMatrixNM weight = FMatrixNM(PrevSize, OutputsNumber);
	weight.RandomInit();

	FMatrixNM bias = FMatrixNM(1, OutputsNumber);
	bias.RandomInit();

	Weights.Push(weight);
	Bias.Push(bias);
}

// Run a Forward pass of the Neural Network Model and output the result
FMatrixNM UNeuralNetworkModel::RunForwardModel(FMatrixNM X)
{
	if (X.GetColumns() != Weights[0].GetRows()) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Input Size Missmatch"));
		return FMatrixNM(0, 0);
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

// Copy a Neural Network Model
void UNeuralNetworkModel::CopyNeuralNetwork(UNeuralNetworkModel* source)
{
	InputsNumber = source->InputsNumber;
	OutputsNumber = source->OutputsNumber;
	HiddenLayers = source->HiddenLayers;

	// Clear any previous weights and bias
	Weights.Empty();
	Bias.Empty();

	for (int k = 0; k < source->Weights.Num(); k++) {
		FMatrixNM weight = source->Weights[k];
		FMatrixNM bias = source->Bias[k];

		Weights.Push(weight);
		Bias.Push(bias);
	}
}

// Mutate the Neural Network
void UNeuralNetworkModel::MutateNeuralNetwork(float mutationrate)
{
	for (int k = 0; k < Weights.Num(); k++) {
		for (int row = 0; row < Weights[k].GetRows(); row++) {
			for (int col = 0; col < Weights[k].GetColumns(); col++) {
				if (FMath::FRand() >= mutationrate) continue;

				Weights[k].SetValue(row, col, FMath::FRandRange(-1.f, 1.f));
			}
		}

		for (int col = 0; col < Bias[k].GetColumns(); col++) {
			if (FMath::FRand() >= mutationrate) continue;

			Bias[k].SetValue(0, col, FMath::FRandRange(-1.f, 1.f));
		}
	}
}