using ITS.Maraspin.EsercizioPreEsame.Api.Models;
using ITS.Maraspin.EsercizioPreEsame.Application.Service_Device;
using ITS.Maraspin.EsercizioPreEsame.Data;
using ITS.Maraspin.EsercizioPreEsame.Data.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace ITS.Maraspin.EsercizioPreEsame.Api.Controllers
{
    [Route("api/file")]
    [ApiController]
    public class FileController : ControllerBase
    {
        private readonly IConfiguration _configuration;
        private readonly IDataAccess _serviceAccess;
        private readonly string _connectionStringDb;
        private readonly string _connectionStringHub;
        private readonly SetDeviceConfiguration _serviceDevice;
        public FileController(IConfiguration configuration, IDataAccess dataAccess )
        {
            _configuration = configuration;
            _connectionStringDb = _configuration.GetConnectionString("db");
            _connectionStringHub = _configuration.GetConnectionString("hub");
            _serviceDevice = new SetDeviceConfiguration(_configuration);
            _serviceAccess = dataAccess;
        }
        // GET: api/<FileController>
        [HttpGet]
        public IEnumerable<FileModelDb> Get()
        {
            return _serviceAccess.GetAll(_connectionStringDb);
        }

        // GET api/<FileController>/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }

        // POST api/<FileController>
        [HttpPost]
        public void Post([FromBody] TwinDevice value)
        {
            _serviceDevice.SetDeviceDesired(value.Desired);
        }

        // PUT api/<FileController>/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/<FileController>/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
