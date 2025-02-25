#include "stdafx.h"
#include "PricePredictor.h"

void ManualAddressInput(std::vector<std::string> &addresses)
{															
	addresses[0] = "C:\\AAIC2019-StocksPredictionData\\FOLD1-eProcessed.csv";
	addresses[1] = "C:\\AAIC2019-StocksPredictionData\\HMRZ1-eProcessed.csv";
	addresses[2] = "C:\\AAIC2019-StocksPredictionData\\IKCO1-eProcessed.csv";
	addresses[3] = "C:\\AAIC2019-StocksPredictionData\\MKBT1-eProcessed.csv";
	addresses[4] = "C:\\AAIC2019-StocksPredictionData\\MSMI1-eProcessed.csv";
	addresses[5] = "C:\\AAIC2019-StocksPredictionData\\PKLJ1-eProcessed.csv";
	addresses[6] = "C:\\AAIC2019-StocksPredictionData\\PTAP1-eProcessed.csv";
	addresses[7] = "C:\\AAIC2019-StocksPredictionData\\SFKZ1-eProcessed.csv";
	addresses[8] = "C:\\AAIC2019-StocksPredictionData\\SGRB1-eProcessed.csv";
	addresses[9] = "C:\\AAIC2019-StocksPredictionData\\SIPA1-eProcessed.csv";
}

void writeTextFile(string fileName)
{
		std::ofstream out("C:\\AAIC2019-StocksPredictionData\\"+fileName+".txt");
		out << ObjectHandler::Instance()->resultOptimizer->GetFinalResults();
		out.close();
}

int main()
{
	string folderAddress;
	vector<double> dataSet;
	vector<string> addresses(10);
	ManualAddressInput(addresses);
	system("DataPreProcessor\\DataPreProcessing.exe");

	for (int i = 0 ; i<10 ; i++)
	{	
		folderAddress = addresses[i];
		double prediction = 0;
		double ErrorSum = 0;
		double maxErr = 0;
		ObjectHandler::Instance()->fileReader->SaveStockData(folderAddress);
		//ObjectHandler::Instance()->fileReader->DisplayPriceData();
		cout << "\nwait for prediction...\n";
		while (!ObjectHandler::Instance()->dataSetProvider->dataSetEnded)
		{
			double error;
			
			
			ObjectHandler::Instance()->dataSetProvider->ProvideNewDataSet(ObjectHandler::Instance()->fileReader->stockPrices, 1);
			dataSet = ObjectHandler::Instance()->dataSetProvider->providedDataSet;
			double realPrice = dataSet[dataSet.size() - 1];
			error = abs(realPrice - prediction);

			//cout << "\t" << prediction << "\t" << error << "\n";
			
			RunAllPredictionAlgorithms(dataSet);

			//cout <<"\t"<< realPrice << "\t" << realPrice - prediction << "\n";
			prediction = ObjectHandler::Instance()->resultOptimizer->GetFinalResults();

			ErrorSum = ErrorSum + error;

			if (error > maxErr)
				maxErr = error;
		}

		//for (int j = 0; j < ObjectHandler::Instance()->newtonExtrapolation->lowErrorDataSets.size(); j++)
		//{
		//	cout << "\n";
		//	for (int i = 0; i < ObjectHandler::Instance()->newtonExtrapolation->lowErrorDataSets[j].size(); i++)
		//	{
		//		cout << "\t" << ObjectHandler::Instance()->newtonExtrapolation->lowErrorDataSets[j][i];
		//	}
		//}
		//cout << "\nlowErrorDataSet Ended";

		//for (int j = 0; j < ObjectHandler::Instance()->newtonExtrapolation->mediumErrorDataSets.size(); j++)
		//{
		//	cout << "\n";
		//	for (int i = 0; i < ObjectHandler::Instance()->newtonExtrapolation->mediumErrorDataSets[j].size(); i++)
		//	{
		//		cout << "\t" << ObjectHandler::Instance()->newtonExtrapolation->mediumErrorDataSets[j][i];
		//	}
		//}
		//cout << "\nmediumErrorDataSet Ended";

		//for (int j = 0; j < ObjectHandler::Instance()->newtonExtrapolation->highErrorDataSets.size(); j++)
		//{
		//	cout << "\n";
		//	for (int i = 0; i < ObjectHandler::Instance()->newtonExtrapolation->highErrorDataSets[j].size(); i++)
		//	{
		//		cout << "\t" << ObjectHandler::Instance()->newtonExtrapolation->highErrorDataSets[j][i];
		//	}
		//}
		cout << "\nhighErrorDataSet Ended";
		cout << "\nmoving to next file\n";
		cout <<"\nNewton Error Sum : "<< ObjectHandler::Instance()->newtonExtrapolation->ErrorSum;
		cout << "\nRegression Error Sum : " << ObjectHandler::Instance()->multipleLinearRegression->ErrorSum;
		//cout << "\nStirling Error Sum : " << ObjectHandler::Instance()->stirlingExtrapolation->ErrorSum;
		cout << "\nOptimized Error Sum : " << ErrorSum;
		
		writeTextFile("out" + to_string(i + 1));

		ObjectHandler::Instance()->RenewAllObjects();
		//cout << ObjectHandler::Instance()->dataSetProvider.numberOfDataSet << "\n";
		//cout << ObjectHandler::Instance()->resultOptimizer.accuracies[ObjectHandler::Instance()->trendDetector.trendType][0] << "\n";
		//cout << ObjectHandler::Instance()->resultOptimizer.accuracies[ObjectHandler::Instance()->trendDetector.trendType][1] << "\n";
		//cout << ObjectHandler::Instance()->resultOptimizer.accuracies[ObjectHandler::Instance()->trendDetector.trendType][2] << "\n";
		//cout << ObjectHandler::Instance()->resultOptimizer.accuracies[ObjectHandler::Instance()->trendDetector.trendType][3] << "\n";
	}

	return 0;
}

void RunAllPredictionAlgorithms(std::vector<double> &dataSet)
{
	//ObjectHandler::Instance()->newtonExtrapolation->RunForwardExtrapolation(dataSet);
	ObjectHandler::Instance()->newtonExtrapolation->RunBackwardExtrapolation(dataSet);
	//ObjectHandler::Instance()->splineExtrapolation->RunExtrapolation(dataSet);	
	//ObjectHandler::Instance()->stirlingExtrapolation->RunExtrapolation(dataSet);
	ObjectHandler::Instance()->multipleLinearRegression->RunExtrapolation(dataSet);
	//if (ObjectHandler::Instance()->dataSetProvider->lastPredictionRealPrice != -1)
	//	ObjectHandler::Instance()->multipleLinearRegression->OptimizeRegressionParameters(ObjectHandler::Instance()->multipleLinearRegression->errorOfLastPrediction);

	ObjectHandler::Instance()->resultOptimizer->SetPredictions();
	ObjectHandler::Instance()->resultOptimizer->SetAccuracies();
	ObjectHandler::Instance()->resultOptimizer->OptimizeFinalResult();
}
