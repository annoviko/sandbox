using Gw2MarketAnalyser;
using System;
using System.Text.Json;

class Program
{
    static void Main()
    {
        Database database = new Database();
        database.LoadRemote();
    }
}