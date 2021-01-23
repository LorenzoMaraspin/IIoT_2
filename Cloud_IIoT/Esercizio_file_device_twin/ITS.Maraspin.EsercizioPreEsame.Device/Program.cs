using ITS.Maraspin.EsercizioPreEsame.Device.Services;
using ITS.Maraspin.EsercizioPreEsame.Application.Service_Device;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.Device
{
    public class Program
    {
        public static void Main(string[] args)
        {
            CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureServices((hostContext, services) =>
                {
                    services.AddHostedService<Worker>();
                    services.AddSingleton<IDeviceService, DeviceService>();
                    services.AddSingleton<ISetDeviceConfiguration, SetDeviceConfiguration>();

                });
    }
}
