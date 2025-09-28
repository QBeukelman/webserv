document.addEventListener("DOMContentLoaded", () => {
	const recordsList = document.getElementById("records-list");

	fetch("../../data/records.jsonl")
		.then(response => {
			if (!response.ok) throw new Error("Could not load records.jsonl");
			return response.text();
		})
		.then(text => {
			const lines = text.trim().split("\n");
			if (lines.length === 0) {
				recordsList.textContent = "No records found.";
				return;
			}

			// Parse each line as JSON
			const records = lines.map(line => {
				try {
					return JSON.parse(line);
				} catch {
					return { error: "Invalid JSON", raw: line };
				}
			});

			recordsList.innerHTML = records
				.map(record => `<pre>${JSON.stringify(record, null, 2)}</pre>`)
				.join("<hr>");
		})
		.catch(err => {
			recordsList.textContent = "Error: " + err.message;
		});
});