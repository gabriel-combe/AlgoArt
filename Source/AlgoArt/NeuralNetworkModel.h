// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeuralNetworkModel.generated.h"


USTRUCT(BlueprintType)
struct FMatrixNN {
	GENERATED_BODY()

private:
	// Number of Rows
	int Rows;

	// Number of Columns
	int Columns;

	// Matrix Data
	TArray<float> Data;

public:
	// Constructor
	FMatrixNN(int rows = 4, int columns = 4) : Rows(rows), Columns(columns)
	{
		Data.SetNumZeroed(Rows * Columns);
	}

	// Randomize the Matrix
	void RandomInit()
	{
		for (int i = 0; i < Data.Num(); i++) {
			Data[i] = FMath::FRandRange(-1.f, 1.f);
		}
	}

	// Get an element of the Matrix
	float GetValue(int row, int col) const
	{
		return Data[col + (row * Columns)];
	}

	// Get the number of Rows
	int GetRows() const
	{
		return Rows;
	}

	// Get the number of Columns
	int GetColumns() const
	{
		return Columns;
	}

	// Set an element of the Matrix
	void SetValue(int row, int col, float val)
	{
		Data[col + (row * Columns)] = val;
	}

	// Apply the Sigmoid function to the Matrix
	void ApplySigmoid()
	{
		for (int i = 0; i < Data.Num(); i++) {
			Data[i] = 1 / (1 + FMath::Exp(-Data[i]));
		}
	}

	// Apply the Hyperbolic Tangent function to the Matrix
	void ApplyTanH()
	{
		for (int i = 0; i < Data.Num(); i++) {
			Data[i] = (FMath::Exp(2 * Data[i]) - 1) / (FMath::Exp(2 * Data[i]) + 1);
		}
	}

	// Apply the ReLU function to the Matrix
	void ApplyReLU()
	{
		for (int i = 0; i < Data.Num(); i++) {
			if (Data[i] < 0.f) Data[i] = 0.f;
		}
	}

	// Apply the Sigmoid function to the Matrix
	static FMatrixNN Sigmoid(FMatrixNN& mat)
	{
		for (int i = 0; i < mat.Data.Num(); i++) {
			mat.Data[i] = 1 / (1 + FMath::Exp(-mat.Data[i]));
		}
	}

	// Apply the Hyperbolic Tangent function to the Matrix
	static FMatrixNN TanH(FMatrixNN& mat)
	{
		for (int i = 0; i < mat.Data.Num(); i++) {
			mat.Data[i] = (FMath::Exp(2 * mat.Data[i]) - 1) / (FMath::Exp(2 * mat.Data[i]) + 1);
		}
	}

	// Apply the ReLU function to the Matrix
	static FMatrixNN ReLU(FMatrixNN& mat)
	{
		for (int i = 0; i < mat.Data.Num(); i++) {
			if (mat.Data[i] < 0.f) mat.Data[i] = 0.f;
		}
	}

	// Overload assignement operator
	void operator=(const FMatrixNN& other)
	{
		Rows = other.Rows;
		Columns = other.Columns;
		Data = other.Data;
	}

	// Overload multiplication operator
	FMatrixNN operator*(const FMatrixNN& other)
	{
		if (Columns != other.Rows) {
			UE_LOG(LogTemp, Error, TEXT("Invalid Matrix Size, Left Matrix columns should be equal to the Right matrix Rows"));
			return FMatrixNN(0, 0);
		}

		FMatrixNN Result = FMatrixNN(Rows, other.Columns);

		for (int row = 0; row < Rows; row++) {
			for (int col = 0; col < other.Columns; col++) {
				float num = 0;
				for (int k = 0; k < Columns; k++) {
					num += GetValue(row, k) * other.GetValue(k, col);
				}

				Result.SetValue(row, col, num);
			}
		}

		return Result;
	}

	// Overload multiplication operator with scalar (left)
	friend FMatrixNN operator*(const float& scalar, const FMatrixNN& other)
	{
		FMatrixNN Result = FMatrixNN(other.Rows, other.Columns);

		for (int i = 0; i < other.Data.Num(); i++) {
			Result.Data[i] = other.Data[i] * scalar;
		}

		return Result;
	}

	// Overload multiplication operator with scalar (right)
	FMatrixNN operator*(const float& scalar)
	{
		FMatrixNN Result = FMatrixNN(Rows, Columns);

		for (int i = 0; i < Data.Num(); i++) {
			Result.Data[i] = Data[i] * scalar;
		}

		return Result;
	}

	// Overload addition operator
	// That function seems so bad that I think I'm going blind just looking at it
	FMatrixNN operator+(const FMatrixNN& other)
	{
		// Full Matrix
		if (Rows == other.Rows && Columns == other.Columns) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int i = 0; i < Data.Num(); i++) {
				Result.Data[i] = Data[i] + other.Data[i];
			}
			return Result;
		}

		// Column Matrix
		if (Rows == other.Rows && other.Columns == 1) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(row, col) + other.GetValue(row, 0));
				}
			}
			return Result;
		}

		if (Rows == other.Rows && Columns == 1) {
			FMatrixNN Result = FMatrixNN(Rows, other.Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < other.Columns; col++) {
					Result.SetValue(row, col, GetValue(row, 0) + other.GetValue(row, col));
				}
			}
			return Result;
		}

		// Row Matrix
		if (Columns == other.Columns && other.Rows == 1) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(row, col) + other.GetValue(0, col));
				}
			}
			return Result;
		}

		if (Columns == other.Columns && Rows == 1) {
			FMatrixNN Result = FMatrixNN(other.Rows, Columns);
			for (int row = 0; row < other.Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(0, col) + other.GetValue(row, col));
				}
			}
			return Result;
		}

		// Error
		UE_LOG(LogTemp, Error, TEXT("Invalid Matrix Size. Matrix should be either the same size, column matrix or row matrix."));
		return FMatrixNN(0, 0);
	}

	// Overload substraction operator
	// That function seems so bad that I think I'm going blind just looking at it
	FMatrixNN operator-(const FMatrixNN& other)
	{
		// Full Matrix
		if (Rows == other.Rows && Columns == other.Columns) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int i = 0; i < Data.Num(); i++) {
				Result.Data[i] = Data[i] - other.Data[i];
			}
			return Result;
		}

		// Column Matrix
		if (Rows == other.Rows && other.Columns == 1) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(row, col) - other.GetValue(row, 0));
				}
			}
			return Result;
		}

		if (Rows == other.Rows && Columns == 1) {
			FMatrixNN Result = FMatrixNN(Rows, other.Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < other.Columns; col++) {
					Result.SetValue(row, col, GetValue(row, 0) - other.GetValue(row, col));
				}
			}
			return Result;
		}

		// Row Matrix
		if (Columns == other.Columns && other.Rows == 1) {
			FMatrixNN Result = FMatrixNN(Rows, Columns);
			for (int row = 0; row < Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(row, col) - other.GetValue(0, col));
				}
			}
			return Result;
		}

		if (Columns == other.Columns && Rows == 1) {
			FMatrixNN Result = FMatrixNN(other.Rows, Columns);
			for (int row = 0; row < other.Rows; row++) {
				for (int col = 0; col < Columns; col++) {
					Result.SetValue(row, col, GetValue(0, col) - other.GetValue(row, col));
				}
			}
			return Result;
		}

		// Error
		UE_LOG(LogTemp, Error, TEXT("Invalid Matrix Size. Matrix should be either the same size, column matrix or row matrix."));
		return FMatrixNN(0, 0);
	}
};

class UNeuralNetworkModel
{
private:
	// Array of Matrix of Weights
	TArray<FMatrixNN> Weights;

	// Array of Matrix of Bias
	TArray<FMatrixNN> Bias;

	// Number of Inputs
	int InputsNumber = 7;

	// Number of Outputs
	int OutputsNumber = 3;

	// Array of size of hidden layers (size of array determines the number of hidden layers)
	TArray<int> HiddenLayers;

public:
	// Initialize the Number of Inputs
	void SetInputsNumber(int number);

	// Initialize the Number of Outputs
	void SetOutputsNumber(int number);

	// Initialize the Config for Hidden Layers
	void SetHiddenLayers(TArray<int>& hiddenlayers);

	// Initialize the Neural Network Model
	void InitModel();

	// Run a Forward pass of the Neural Network Model and output the result
	FMatrixNN RunForwardModel(FMatrixNN Input);
};
