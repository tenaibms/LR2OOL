#include <math.h>
#include "statistics.h"

statistics::OnlineMean::OnlineMean()
{
	accumulator = 0;
	dataset_size = 0;
}

void statistics::OnlineMean::Insert(int data)
{
	accumulator += data;
	++dataset_size;
}

double statistics::OnlineMean::GetMean()
{
	return accumulator / (double)dataset_size;
}

statistics::OnlineStandardDeviation::OnlineStandardDeviation()
{
	mean = 0;
	m2 = 0;
	dataset_size = 0;
}

void statistics::OnlineStandardDeviation::Insert(int data)
{
	++dataset_size;
	double delta = data - mean;
	mean += delta / dataset_size;
	m2 += delta * (data - mean);
}

double statistics::OnlineStandardDeviation::GetPopulationStandardDeviation()
{
	if (dataset_size < 1)
		return 0;
	return sqrt(m2 / (dataset_size));
}

double statistics::OnlineStandardDeviation::GetSampleStandardDeviation()
{
	if (dataset_size < 2)
		return 0;
	return sqrt(m2 / (dataset_size - 1));
}

statistics::Ema::Ema()
{
	ema = 0;
}

void statistics::Ema::Reset()
{
	ema = 0;
}

void statistics::Ema::Insert(float value)
{
	ema = ema + (alpha * (value - ema));
}
