using System.Text.Json;


namespace Gw2MarketAnalyser
{
    internal class RestClient
    {
        static readonly string Host = "https://api.guildwars2.com";

        static readonly string EndpointGetItems = "/v2/items";

        public static Dictionary<ulong, string>? GetItems()
        {
            List<ulong>? ids = GetItemIds();
            if (ids == null)
            {
                return null;
            }

            Dictionary<ulong, string> items = new Dictionary<ulong, string>();
            Console.WriteLine($"The number of items to load: '{ids.Count}'.");

            ulong counter = 0;
            List<ulong> ids_to_request = new List<ulong>();
            for (int j = 0; j < ids.Count; j++)
            {
                ulong id = ids[j];

                counter++;
                ids_to_request.Add(id);

                /* ANet allows to load 200 maximum elements at once */
                if ((ids_to_request.Count != 200) && (j != (ids.Count - 1)))
                {
                    continue;
                }

                List<string> names = GetItemNames(ids_to_request);
                if (names.Count == 0)
                {
                    Console.WriteLine("Error: item names were not obtained.");
                    continue;
                }

                for (int i = 0; i < names.Count; i++)
                {
                    items.Add(ids_to_request[i], names[i]);
                }

                ids_to_request.Clear();

                if (counter % 1000 == 0)
                {
                    Console.WriteLine($"Loading is in progress... loaded '{items.Count}' items.");
                }
            }

            Console.WriteLine($"Loaded '{counter}' items.");
            return items;
        }

        public static List<ulong>? GetItemIds()
        {
            string? items = ExecuteGetRequest(EndpointGetItems);
            if (items == null)
            {
                return null;
            }

            return JsonSerializer.Deserialize<List<ulong>>(items);
        }

        private static List<string>? GetItemNames(List<ulong> ids)
        {
            string endpoint = GetEndpointItemsByIds(ids);
            string? response = ExecuteGetRequest(endpoint);

            if (response == null)
            {
                return new List<string>();
            }

            using (JsonDocument doc = JsonDocument.Parse(response))
            {
                JsonElement root = doc.RootElement;

                List<string> result = new List<string>();

                if (root.ValueKind == JsonValueKind.Array)
                {
                    foreach (JsonElement element in root.EnumerateArray())
                    {
                        if (element.TryGetProperty("name", out JsonElement nameElement))
                        {
                            string name = nameElement.GetString();
                            result.Add(name);
                        }
                    }
                }
                else
                {
                    Console.WriteLine("Error: the response was expected as a list!");
                    Console.Write(response);
                }

                return result;
            }
        }

        private static string? GetItemName(ulong id)
        {
            string endpoint = GetEndpointItemById(id);
            string? response = ExecuteGetRequest(endpoint);
            if (response == null)
            {
                return null;
            }

            using (JsonDocument doc = JsonDocument.Parse(response))
            {
                JsonElement root = doc.RootElement;

                if (root.TryGetProperty("name", out JsonElement nameElement))
                {
                    string name = nameElement.GetString();
                    return name;
                }
                else
                {
                    Console.WriteLine("Error: field 'name' not found in JSON.");
                }
            }

            return null;
        }

        private static string GetEndpointItemsByIds(List<ulong> ids)
        {
            string endpoint = EndpointGetItems;
            bool is_first = true;

            foreach (ulong id in ids)
            {
                if (!is_first)
                {
                    endpoint += ",";
                }
                else
                {
                    endpoint += "?ids=";
                }
                endpoint += id.ToString();
                is_first = false;
            }

            return endpoint;
        }

        private static string GetEndpointItemById(ulong id)
        {
            return EndpointGetItems + "?id=" + id.ToString();
        }

        private static string? ExecuteGetRequest(string endpoint)
        {
            string apiUrl = Host + endpoint;

            try
            {
                using (HttpClient client = new HttpClient())
                {
                    HttpResponseMessage response = client.GetAsync(apiUrl).Result;

                    if (response.IsSuccessStatusCode)
                    {
                        return response.Content.ReadAsStringAsync().Result;
                    }
                    else
                    {
                        Console.WriteLine($"Impossible to get response '{endpoint}' (status code: {response.StatusCode}).");
                    }
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine($"Impossible to execute GET request: '{exception.Message}'.");
            }

            return null;
        }
    }
}
