using System;
using System.Collections.Generic;
using System.Text;

namespace ITS.Maraspin.Fridge.ServiceBus.Model
{
    public class FridgeClass
    {
        public string FridgeId { get; set; }
        public double RealTemperature { get; set; }
        public double SetTemperature { get; set; }
        public DateTime Date { get; set; }
    }
}
