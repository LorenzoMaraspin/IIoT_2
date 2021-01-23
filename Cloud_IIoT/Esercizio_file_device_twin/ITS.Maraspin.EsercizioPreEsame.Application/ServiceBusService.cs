using ITS.Maraspin.EsercizioPreEsame.Application.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Azure.Messaging.ServiceBus;
using Microsoft.Extensions.Configuration;
using System.Text.Json;
using System.Diagnostics;

namespace ITS.Maraspin.EsercizioPreEsame.Application
{
    public class ServiceBusService : IMessageService
    {
        ServiceBusProcessor processor;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        private readonly string _topicName;
        private readonly string _subscriptionName;

        public ServiceBusService(IConfiguration configuration)
        {
            _configuration = configuration;
            _connectionString = configuration.GetConnectionString("ServiceBus");
            _topicName = configuration["TopicName"] ?? "file";
            _subscriptionName = configuration["SubscriptionName"];
        }
        public async Task StartReceiveMessagesFromSubscriptionAsync(Action<FileModelReceive> processMessageFunc)
        {
            if (string.IsNullOrWhiteSpace(_subscriptionName))
                throw new ArgumentNullException("Parameter SubscriptionName cannot be null or empty");

            ServiceBusClient client = new ServiceBusClient(_connectionString);

            processor = client.CreateProcessor(_topicName, _subscriptionName, new ServiceBusProcessorOptions());
            processor.ProcessMessageAsync += async args =>
            {
                string body = args.Message.Body.ToString();
                var message = JsonSerializer.Deserialize<FileModelReceive>(body);
                processMessageFunc.Invoke(message);
                await args.CompleteMessageAsync(args.Message);
            };
            processor.ProcessErrorAsync += ErrorHandler;
            await processor.StartProcessingAsync();
        }

        public async Task StopReceiveMessagesFromSubscriptionAsync()
        {
            if (processor != null)
                await processor.StopProcessingAsync();
        }

        static Task ErrorHandler(ProcessErrorEventArgs args)
        {
            Debug.WriteLine(args.Exception.ToString());
            return Task.CompletedTask;
        }
    }
}
