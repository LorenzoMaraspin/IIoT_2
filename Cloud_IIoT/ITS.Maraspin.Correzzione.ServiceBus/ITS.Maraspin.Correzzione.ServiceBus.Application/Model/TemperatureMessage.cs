using System;
using System.Collections.Generic;
using System.Text;

namespace ITS.Maraspin.Correzzione.ServiceBus.Application.Model
{
    public class TemperatureMessage
    {
        public int DeviceId { get; set; }
        public double TemperatureDesired { get; set; }
        public double TemperatureMeasured { get; set; }
        public DateTime MeasurementdData { get; set; }
    }
}
