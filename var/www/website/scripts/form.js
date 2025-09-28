document.addEventListener("DOMContentLoaded", () => {
	const form = document.querySelector(".form-box");
	const messageDiv = document.getElementById("form-message");

	form.addEventListener("submit", async (e) => {
		e.preventDefault();

		const formData = new FormData(form);

		try {
			const response = await fetch(form.action, {
				method: form.method,
				body: formData
			});

			if (response.ok) {
				messageDiv.textContent = "Success! Your form was submitted.";
				messageDiv.className = "status-message success";
			} else {
				messageDiv.textContent = "Failed to submit form.";
				messageDiv.className = "status-message error";
			}
		} catch (err) {
			messageDiv.textContent = "Error connecting to server.";
			messageDiv.className = "status-message warn";
		}
	});
});
