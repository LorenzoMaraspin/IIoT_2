using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace ITS.Maraspin.Correzzione.ServiceBus.Web.Model
{
    public class DeviceTemperatureModel
    {
        [Required]
        public int DeviceId { get; set; }
        [Required]
        public double TemperatureDesired { get; set; }
        [Required]
        public double TemperatureMeasured { get; set; }
        [Required]
        public DateTime MeasurementdData { get; set; }
    }
}
