using Azure.Messaging.ServiceBus;
using ITS.Maraspin.Correzzione.ServiceBus.Application.Model;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Maraspin.Correzzione.ServiceBus.Application
{
    public interface IMessageService
    {
        Task SendData(TemperatureMessage message);
        Task StartReceiveMessageFromSubscriptionAsync(Action<ProcessMessageEventArgs> processMessageFunc);
        Task StopReceiveMessageFromSubscriptionAsync();
    }
}
