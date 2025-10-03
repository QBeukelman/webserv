/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   upload.js                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/30 13:51:54 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/03 11:28:15 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("upload-form");
	const statusEl = document.getElementById("upload-message");

	if (!form) {
		console.error("upload.js: #upload-form not found");
		return;
	}

	form.addEventListener("submit", async (e) => {
		e.preventDefault();

		const formData = new FormData(form);

		statusEl.textContent = "Uploading...";
		statusEl.className = "status-message";

		try {
			const res = await fetch(form.action, {
				method: "POST",
				body: formData
			});

			if (res.ok) {
				statusEl.textContent = "[201] Upload successful.";
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
