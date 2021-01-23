using ITS.Maraspin.EsercizioPreEsame.Device.Models;
using Microsoft.Azure.Devices.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.Device.Services
{
    public class DeviceService : IDeviceService
    {
        public DeviceClient client;
        public async void InitClient(string connectionString)
        {
            try
            {
                Console.WriteLine("Connecting to hub");
                client = DeviceClient.CreateFromConnectionString(connectionString);
            }
            catch (Exception ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error in sample: {0}", ex.Message);
            }
        }
        public async Task sendMessage(FileModel model)
        {
            var messageToWrite = new Message(System.Text.Encoding.UTF8.GetBytes(JsonSerializer.Serialize(model)));
            await client.SendEventAsync(messageToWrite);
        }

        public async Task<string> GetDeviceTwin()
        {
            Console.WriteLine("Retrieving twin");
            var twin = await client.GetTwinAsync();
            return twin.Properties.Desired["Filename"];
        }
    }
}
