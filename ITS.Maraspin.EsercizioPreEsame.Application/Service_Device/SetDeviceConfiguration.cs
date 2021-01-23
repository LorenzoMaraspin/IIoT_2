using Microsoft.Azure.Devices;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.Text;

namespace ITS.Maraspin.EsercizioPreEsame.Application.Service_Device
{
    public class SetDeviceConfiguration : ISetDeviceConfiguration
    {
        private readonly RegistryManager _registryManager;
        private readonly string _connectionString;
        private readonly string _device = "device-maraspin-1";
        private readonly IConfiguration _configuration;

        public SetDeviceConfiguration(IConfiguration configuration)
        {
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("hub");
            _registryManager = RegistryManager.CreateFromConnectionString(_connectionString);
        }
        public async void SetDeviceDesired(string filename)
        {
            var twin = await _registryManager.GetTwinAsync(_device);
            twin.Properties.Desired["Filename"] = filename;

            await _registryManager.UpdateTwinAsync(twin.DeviceId, twin, twin.ETag);
        }


    }
}
