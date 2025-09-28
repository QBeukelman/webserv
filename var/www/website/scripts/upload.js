document.addEventListener("DOMContentLoaded", () => {
	const fileInput = document.getElementById("file");
	const fileInfo = document.getElementById("file-info");

	fileInput.addEventListener("change", () => {
		if (fileInput.files.length > 0) {
			fileInfo.textContent = `Selected: ${fileInput.files[0].name}`;
		} else {
			fileInfo.textContent = "";
		}
	});
});