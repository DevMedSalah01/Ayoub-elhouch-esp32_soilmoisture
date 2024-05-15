// // const ledElement = document.getElementById('led');
// // const ledButton = document.getElementById('led-button');

// // // Fonction pour envoyer une requête CoAP pour contrôler la LED
// // function toggleLed() {
// //   const ledState = ledElement.classList.contains('on') ? '0' : '1';

// //   fetch('coap://localhost/led', {
// //     method: 'PUT',
// //     body: ledState
// //   })
// //   .then(response => response.text())
// //   .then(data => {
// //     console.log('LED state updated:', data);
// //     ledElement.classList.toggle('on');
// //   })
// //   .catch(error => {
// //     console.error('Error toggling LED:', error);
// //   });
// // }

// // ledButton.addEventListener('click', toggleLed);
// // fetch('/api/soil-moisture')
// //   .then(response => response.json())
// //   .then(data => {
// //     const ctx = document.getElementById('soil-moisture-chart').getContext('2d');
// //     new Chart(ctx, {
// //       type: 'line',
// //       data: {
// //         labels: data.map(item => new Date(item.timestamp).toLocaleString()),
// //         datasets: [{
// //           label: 'Humidité du sol',
// //           data: data.map(item => item.soilMoisture),
// //           borderColor: 'blue',
// //           fill: false
// //         }]
// //       },
// //       options: {
// //         responsive: true,
// //         maintainAspectRatio: false,
// //         scales: {
// //           y: {
// //             min: 0,
// //             max: 4095,
// //             ticks: {
// //               stepSize: 500
// //             }
// //           }
// //         }
// //       }
// //     });
// //   })
// //   .catch(error => {
// //     console.error('Erreur lors de la récupération des données :', error);
// //   });

// // const ledElement = document.getElementById('led');
// // const ledButton = document.getElementById('led-button');

// // // Fonction pour envoyer une requête CoAP pour contrôler la LED
// // // script.js
// // function updateSoilMoistureValue(soilMoisture) {
// //   document.getElementById('soilMoistureValue').textContent = soilMoisture;
// // }

// // Fonction qui récupère les données de l'API REST
// // function fetchSoilMoistureData() {
// //   fetch('http://localhost:3001/api/soil-moisture')
// //     .then(response => response.json())
// //     .then(data => {
// //       updateSoilMoistureValue(data.soilMoisture);
// //       setTimeout(fetchSoilMoistureData, 10000); // Récupérer les données toutes les 10 secondes
// //     })
// //     .catch(error => {
// //       console.error('Error fetching soil moisture data:', error);
// //       setTimeout(fetchSoilMoistureData, 10000); // Réessayer dans 10 secondes
// //     });
// // }

// // // Lancer la récupération des données
// // fetchSoilMoistureData();
// // script.js
// function updateSoilMoistureValue(soilMoisture) {
//   console.log('Updating soil moisture value:', soilMoisture);
//   document.getElementById('soilMoistureValue').textContent = soilMoisture;
// }

// function fetchSoilMoistureData() {
//   console.log('Fetching soil moisture data...');
//   fetch('http://localhost:3001/api/soil-moisture')
//     .then(response => response.json())
//     .then(data => {
//       console.log('Received soil moisture data:', data);
//       updateSoilMoistureValue(data.soilMoisture);
//       setTimeout(fetchSoilMoistureData, 10000); // Récupérer les données toutes les 10 secondes
//     })
//     .catch(error => {
//       console.error('Error fetching soil moisture data:', error);
//       document.getElementById('soilMoistureValue').textContent = 'Error';
//       setTimeout(fetchSoilMoistureData, 10000); // Réessayer dans 10 secondes
//     });
// }

// // Lancer la récupération des données
// fetchSoilMoistureData();
// script.js
function updateSoilMoistureValue(soilMoisture) {
  console.log('Updating soil moisture value:', soilMoisture);
  document.getElementById('soilMoistureValue').textContent = soilMoisture;
}

function fetchSoilMoistureData() {
  console.log('Fetching soil moisture data...');
  fetch('http://192.168.1.19')
    .then(response => response.json())
    .then(data => {
      console.log('Received soil moisture data:', data);
      updateSoilMoistureValue(data.soilMoisture);
      setTimeout(fetchSoilMoistureData, 10000); // Fetch data every 10 seconds
    })
    .catch(error => {
      console.error('Error fetching soil moisture data:', error);
      document.getElementById('soilMoistureValue').textContent = 'Error';
      setTimeout(fetchSoilMoistureData, 10000); // Retry in 10 seconds
    });
}

// Start fetching data
fetchSoilMoistureData();