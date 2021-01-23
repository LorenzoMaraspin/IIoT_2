using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Azure.Messaging.ServiceBus;
using Microsoft.Extensions.Configuration;
using System.Text.Json;
using ITS.Maraspin.Fridge.ServiceBus.Model;
using Microsoft.Azure.Cosmos.Table;
using ITS.Maraspin.Fridge.ServiceBus.Data;

namespace ITS.Maraspin.Fridge.ServiceBus.Service
{
    public class BusService
    {
        private readonly string _connectionString;
        private readonly string _connectionStringStorage;
        private readonly string _topic;
        private readonly IConfiguration _configuration;
        private TableService service;

        public BusService(string connectionString)
        {
            _connectionString = connectionString;
            _topic = "fridge";
        }
        public BusService(string connectionString, string connectionStringStorage)
        {
            _connectionString = connectionString;
            _connectionStringStorage = connectionStringStorage;
            _topic = "fridge";
        }
        public async Task Send(string value)
        {
            await using(ServiceBusClient client = new ServiceBusClient(_connectionString))
            {
                ServiceBusSender sender = client.CreateSender(_topic);
                ServiceBusMessage message = new ServiceBusMessage(value);
                await sender.SendMessageAsync(message);
            }
        }
        public async Task MessageHandler(ProcessMessageEventArgs args)
        {
            string body = args.Message.Body.ToString();
            var json = JsonSerializer.Deserialize<FridgeClass>(body);
            Console.WriteLine($"Received: {body}");

            FridgeEntity fridge = new FridgeEntity(json.FridgeId);
            fridge.Date = json.Date;
            fridge.FridgeId = json.FridgeId;
            fridge.RealTemperature = json.RealTemperature;
            fridge.SetTemperature = json.SetTemperature;

            service = new TableService(_connectionStringStorage);
            service.SaveData(fridge);


            // complete the message. messages is deleted from the queue. 
            await args.CompleteMessageAsync(args.Message);
        }
        public async Task<Task> ErrorHandler(ProcessErrorEventArgs args)
        {
            Console.WriteLine(args.Exception.ToString());
            return Task.CompletedTask;
        }
        public async Task Receive(string sub)
        {
            await using (ServiceBusClient client = new ServiceBusClient(_connectionString))
            {
                // create a processor that we can use to process the messages
                ServiceBusProcessor processor = client.CreateProcessor("fridge",sub , new ServiceBusProcessorOptions());

                // add handler to process messages
                processor.ProcessMessageAsync += MessageHandler;

                // add handler to process any errors
                processor.ProcessErrorAsync += ErrorHandler;

                // start processing 
                await processor.StartProcessingAsync();

                Console.WriteLine("Wait for a minute and then press any key to end the processing");
                Console.ReadKey();

                // stop processing 
                Console.WriteLine("\nStopping the receiver...");
                await processor.StopProcessingAsync();
                Console.WriteLine("Stopped receiving messages");
            }
        }

    }
}
