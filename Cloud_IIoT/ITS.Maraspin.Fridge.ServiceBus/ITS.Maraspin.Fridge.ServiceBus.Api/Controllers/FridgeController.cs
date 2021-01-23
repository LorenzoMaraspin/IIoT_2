using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ITS.Maraspin.Fridge.ServiceBus.Model;
using ITS.Maraspin.Fridge.ServiceBus.Service;
using Microsoft.Extensions.Configuration;
using System.Text.Json;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace ITS.Maraspin.Fridge.ServiceBus.Api.Controllers
{
    [Route("api/fridge")]
    [ApiController]
    public class FridgeController : ControllerBase
    {
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        public FridgeController(IConfiguration configuration)
        {
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("bus");
        }
        // GET: api/<FridgeController>
        [HttpGet]
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET api/<FridgeController>/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }

        // POST api/<FridgeController>
        [HttpPost]
        public void Post([FromBody] FridgeClass value)
        {
            string json = JsonSerializer.Serialize(value);
            BusService service = new BusService(_connectionString);
            service.Send(json);

        }

        // PUT api/<FridgeController>/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/<FridgeController>/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
