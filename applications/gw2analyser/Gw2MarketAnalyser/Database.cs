using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Collections;

namespace Gw2MarketAnalyser
{
    internal class Database
    {
        public static readonly string DictIdToNameFileName = "JsonDatabaseItems.db";

        private Dictionary<ulong, string> dictIdToName = new Dictionary<ulong, string>();

        public void LoadRemote()
        {
            LoadDictItemIds();
        }

        public void LoadLocal()
        {
            LoadDictItemIds();
        }

        private void LoadDictItemIds()
        {
            if (File.Exists(DictIdToNameFileName))
            {
                Console.WriteLine("Loading database from the file...");
                LoadDictItemIdsFromFile();
                Console.WriteLine($"Items database loaded from the file (entries: '{dictIdToName.Count}').");
            }
            else
            {
                Console.WriteLine("Loading database from the server...");
                LoadDictItemIdsFromServer();
                Console.WriteLine($"Items database loaded from the server (entries: '{dictIdToName.Count}').");
            }
        }

        private void LoadDictItemIdsFromServer()
        {
            dictIdToName = RestClient.GetItems();
            if (dictIdToName == null)
            {
                throw new Exception("Impossible to load items from GW2 server.");
            }

            SaveDictItemIdsToFile();
        }

        private void LoadDictItemIdsFromFile()
        {
            string jsonString = File.ReadAllText(DictIdToNameFileName);
            dictIdToName = JsonSerializer.Deserialize<Dictionary<ulong, string>>(jsonString);
        }

        private void SaveDictItemIdsToFile()
        {
            string jsonString = JsonSerializer.Serialize(dictIdToName);
            File.WriteAllText(DictIdToNameFileName, jsonString);
        }
    }
}
