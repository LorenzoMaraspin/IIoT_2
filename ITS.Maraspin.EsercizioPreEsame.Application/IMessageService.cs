using ITS.Maraspin.EsercizioPreEsame.Application.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.Application
{
    public interface IMessageService
    {
        Task StartReceiveMessagesFromSubscriptionAsync(Action<FileModelReceive> processMessageFunc);

        Task StopReceiveMessagesFromSubscriptionAsync();
    }
}
