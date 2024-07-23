using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gw2MarketAnalyser
{
    internal class DatabaseEntry
    {
        public ulong id;
        public string? name;
        public DatabaseEntry? buy;
        public DatabaseEntry? sell;
    }
}
