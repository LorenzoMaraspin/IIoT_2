using ITS.Maraspin.EsercizioPreEsame.Application;
using ITS.Maraspin.EsercizioPreEsame.FileMessage.Models;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.FileMessage
{
    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;
        private readonly IMessageService _messageService;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;

        public Worker(ILogger<Worker> logger, IMessageService messageService, IConfiguration configuration)
        {
            _logger = logger;
            _messageService = messageService;
            _configuration = configuration;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            MessageFileModel model = new MessageFileModel();
            await _messageService.StartReceiveMessagesFromSubscriptionAsync(
                message =>
                {
                    _logger.LogInformation("DEVICE: {0}, TEMP: {1}", message.FileName, message.Message);
                    model.FileName = message.FileName;
                    model.Message = message.Message;
                    model.WriteFile(model.FileName, model.Message);
                });
            while (!stoppingToken.IsCancellationRequested)
            {
                _logger.LogInformation("Worker running at: {time}", DateTimeOffset.Now);
                await Task.Delay(10000, stoppingToken);
            }
        }
        public override async Task StopAsync(CancellationToken cancellationToken)
        {
            await _messageService.StopReceiveMessagesFromSubscriptionAsync();
            await base.StopAsync(cancellationToken);
        }
    }
}
