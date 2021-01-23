using Microsoft.Azure.Cosmos.Table;
using System;
using System.Collections.Generic;
using System.Text;

namespace ITS.Maraspin.Fridge.ServiceBus.Data
{
    public class FridgeEntity : TableEntity
    {
        public FridgeEntity()
        {

        }
        public FridgeEntity(string id)
        {
            PartitionKey = "ITS";
            RowKey = id;
        }
        public string FridgeId { get; set; }
        public double RealTemperature { get; set; }
        public double SetTemperature { get; set; }
        public DateTime Date { get; set; }
    }
}
