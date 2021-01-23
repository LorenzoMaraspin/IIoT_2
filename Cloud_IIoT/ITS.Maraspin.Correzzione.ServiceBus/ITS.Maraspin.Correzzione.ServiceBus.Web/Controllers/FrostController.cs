using ITS.Maraspin.Correzzione.ServiceBus.Application;
using ITS.Maraspin.Correzzione.ServiceBus.Application.Model;
using ITS.Maraspin.Correzzione.ServiceBus.Web.Model;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace ITS.Maraspin.Correzzione.ServiceBus.Web.Controllers
{
    [Route("api/frost")]
    [ApiController]
    public class FrostController : ControllerBase
    {
        private readonly IMessageService _messageService;

        public FrostController(IMessageService messageService)
        {
            _messageService = messageService;
        }
        // GET api/frost/[id]
        [HttpGet("{id}")]
        public IActionResult GetAllTemperatures(int id)
        {
            throw new NotImplementedException();
        }
        [HttpPost]
        public async Task<IActionResult> SaveTemperature(DeviceTemperatureModel model)
        {
            try
            {
                var message = new TemperatureMessage();
                message.DeviceId = model.DeviceId;
                message.MeasurementdData = model.MeasurementdData;
                message.TemperatureDesired = model.TemperatureDesired;
                message.TemperatureMeasured = model.TemperatureMeasured;

                await _messageService.SendData(message);
                return Ok();

            }catch (Exception e)
            {
                return StatusCode(500, new { Message = e.Message });
            }
        }
    }
}
