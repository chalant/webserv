import requests

def upload_file(file_path, url):
    # Open the file in binary mode
    with open(file_path, 'rb') as file:
        # Define the files dictionary to send in the POST request
        files = {'file': file}

        # Make the POST request to upload the file
        response = requests.post(url, files=files)

        # Print the response from the server
        print(f"Status Code: {response.status_code}")
        print(f"Response Text: {response.text}")

if __name__ == "__main__":
    # Path to the file you want to upload
    file_path = 'UploadResponseGenerator_test_main.cpp'
    
    # The URL to which the file will be uploaded
    upload_url = 'http://localhost:8080/upload'

    # Call the function to upload the file
    upload_file(file_path, upload_url)
