using ITS.Maraspin.EsercizioPreEsame.Application;
using ITS.Maraspin.EsercizioPreEsame.Data;
using ITS.Maraspin.EsercizioPreEsame.Data.Models;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.StorageDb
{
    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;
        private readonly IMessageService _messageService;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        private readonly IDataAccess _serviceStorage;
        public Worker(ILogger<Worker> logger, IMessageService messageService, IConfiguration configuration, IDataAccess serviceStorage)
        {
            _logger = logger;
            _messageService = messageService;
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("db");
            _serviceStorage = serviceStorage;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            FileModelDb dbModel = new FileModelDb();
            await _messageService.StartReceiveMessagesFromSubscriptionAsync(
                message =>
                {
                    _logger.LogInformation("DEVICE: {0}, TEMP: {1}", message.FileName, message.Message);
                    dbModel.Filename = message.FileName;
                    dbModel.Message = message.Message;
                    //Storage
                    _serviceStorage.Insert(dbModel,_connectionString);

                });

            while (!stoppingToken.IsCancellationRequested)
            {
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
