#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include <string>


class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void gotoNextTimeframe();
        void processUserOption(std::string userOption);
       
        bool isProductValid(std::string type);
        bool isAskOrBid(std::string type);
        bool isWhitespace(std::string s);
        bool isTokensSecondElementValid(std::string tokensTwo);
        bool isMaxOrMin(std::string type);
        bool isTimestampsEnough(int NumOfTimestamps);
        int findIndexOfCurrentTimestamp();

        void printKnownProducts();
        void printCurrentTime(); 
        void helpCmd(std::vector<std::string>tokens);
        void printTimestamps();
        void printMin(std::vector<std::string>tokens);
        void printMax(std::vector<std::string>tokens);
        void printAvg(std::vector<std::string>tokens);
        void printMedian(std::vector<std::string>tokens);
        void printPredict(std::vector<std::string>tokens);
     
        double getAvg(OrderBookType cmdType, std::string cmdProduct, int cmdNumOfTimestamps); 
        double getMedianNumber(std::vector<double> theArray , int size);
        double getPredict(std::vector<double> formula, double x);
        double useMaxOrMin(std::string cmdMaxOrMin, std::vector<OrderBookEntry> ordersFiltered);

        std::vector<double> getPrices(OrderBookType cmdType, std::string cmdProduct, int cmdNumOfTimestamps);
        std::vector<double> getYValues(std::string cmdMaxOrMin, std::string cmdProduct, OrderBookType cmdType);
        std::vector<int> getXValues(int NumOfTimestamps);
        std::vector<double> fitLinearRegression(std::vector<double>yValues, std::vector<int>xValue);
        std::vector<std::string>findRequiredTimestamps(int timesteps);

        std::string getUserOption();
        std::string currentTime;
        // int IndexOfCurrentTimestamp;
      
        
        OrderBook orderBook{"20200601.csv"}; // instantiating the class OrderBook;
	    // OrderBook orderBook{"test.csv"};
     

};
