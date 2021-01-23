using ITS.Maraspin.Correzzione.ServiceBus.Application.Model;
using System;
using System.Collections.Generic;
using System.Text;
using Azure.Messaging.ServiceBus;
using Microsoft.Extensions.Configuration;
using System.Threading.Tasks;
using System.Text.Json;
using System.Diagnostics;

namespace ITS.Maraspin.Correzzione.ServiceBus.Application.Services
{
    public class ServiceBusService : IMessageService
    {
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        private readonly string _topicName;
        private readonly string _subscriptionName;

        ServiceBusProcessor processor;

        public ServiceBusService(IConfiguration configuration)
        {
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("ServiceBus");
            _topicName = configuration["TopicName"] ?? "Fridge";
            _subscriptionName = configuration["SubscriptionName"];
        }
        public async Task SendData(TemperatureMessage message)
        {
            var data = JsonSerializer.Serialize(message);
            await using (ServiceBusClient client = new ServiceBusClient(_connectionString));
            // create a sender for the queue 
            ServiceBusSender sender = client.CreateSender(_topicName);
            // send the message
            await sender.SendMessageAsync(new ServiceBusMessage(data));
            Console.WriteLine($"Sent a single message to the queue: {_topicName}");
        }
        public async Task StartReceiveMessageFromSubscriptionAsync(Action<ProcessMessageEventArgs> processMessageFunc)
        {
            if(string.IsNullOrWhiteSpace(_subscriptionName))
            {
                throw new ArgumentNullException("Parameter SubscriptionName");
            }
            await using (ServiceBusClient client = new ServiceBusClient(_connectionString))
            {
                // create a processor that we can use to process the messages
                processor = client.CreateProcessor(_topicName, _subscriptionName);

                //processor.ProcessMessageAsync += processMessageFunc;
                processor.ProcessErrorAsync += ErrorHandler;

                await processor.StartProcessingAsync();
            }
        }
        public async Task StopReceiveMessageFromSubscriptionAsync()
        {
            if (processor != null)
                await processor.StopProcessingAsync();
        }
        // handle any errors when receiving messages
        Task ErrorHandler(ProcessErrorEventArgs args)
        {
            Debug.WriteLine(args.Exception.ToString());
            return Task.CompletedTask;
        }
    }
}
