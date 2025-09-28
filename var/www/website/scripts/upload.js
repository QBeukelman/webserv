// ../../website/scripts/upload.js
document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("upload-form");
	const statusEl = document.getElementById("status-message");

	if (!form) {
		console.error("upload.js: #upload-form not found");
		return;
	}

	form.addEventListener("submit", async (e) => {
		e.preventDefault(); // <-- stops the page navigation

		const formData = new FormData(form);

		// Optional: reset message
		statusEl.textContent = "Uploading…";
		statusEl.className = "status-message pending";

		try {
			const res = await fetch(form.action, {
				method: "POST",
				body: formData
			});

			if (res.ok) {
				statusEl.textContent = "Upload successful!";
				statusEl.className = "status-message success";
			} else {
				statusEl.textContent = `Upload failed (${res.status})`;
				statusEl.className = "status-message error";
			}
		} catch (err) {
			statusEl.textContent = "Could not reach the server.";
			statusEl.className = "status-message warn";
			console.error(err);
		}
	});
});
