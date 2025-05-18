// Predefined results for each paper
const resultsData = {
    paper1: {
        title: "The worst-case time complexity for generating all maximal cliques (Tomita et al.)",
        totalCliques: [460184, 226858, 450631],
        cliqueSizes: [17, 20, 42],
        execTimes: [686890, 2189789, 93782037]
    },
    paper2: {
        title: "Listing All Maximal Cliques in Sparse Graphs in Near-optimal Time (Eppstein et al.)",
        totalCliques: [460185, 226859, 450631],
        cliqueSizes: [17, 20, 42],
        execTimes: [730395, 560722, 75603216]
    },
    paper3: {
        title: "Arboricity and Subgraph Listing Algorithms",
        totalCliques: [459002, 226859, 450631],
        cliqueSizes: [17, 20, 42],
        execTimes: [1687990, 5573290, 118007992]
    }
};

// Load initial results
window.onload = function () {
    showResults();
};

// Display selected paper results + graphs
function showResults() {
    const paper = document.getElementById('paper-select').value;
    const data = resultsData[paper];

    document.getElementById('results-display').innerHTML = `
        <h3>${data.title}</h3>
        <ul>
            <li><strong>Wiki-Vote.txt:</strong> ${data.totalCliques[0]} cliques, Max Size: ${data.cliqueSizes[0]}, Time: ${data.execTimes[0]} ms</li>
            <li><strong>Email-Enron.txt:</strong> ${data.totalCliques[1]} cliques, Max Size: ${data.cliqueSizes[1]}, Time: ${data.execTimes[1]} ms</li>
            <li><strong>As-Skitter.txt:</strong> ${data.totalCliques[2]} cliques, Max Size: ${data.cliqueSizes[2]}, Time: ${data.execTimes[2]} ms</li>
        </ul>
    `;

    // Plot graphs for selected paper
    plotCharts(data);
}
function createChart(canvasId, labels, data, title) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    new Chart(ctx, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [{
                label: title,
                data: data,
                backgroundColor: ['#3498db', '#e74c3c', '#2ecc71'],
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            scales: {
                y: { beginAtZero: true }
            }
        }
    });
}


