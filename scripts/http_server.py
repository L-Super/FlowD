import http.server
import socketserver
import os
import sys


def run(port, directory, bind='127.0.0.1'):
    # 切换到下载目录
    os.chdir(directory)

    # 创建请求处理器和TCP服务器
    handler = http.server.SimpleHTTPRequestHandler
    with socketserver.TCPServer(("", port), handler) as httpd:
        print(f"Serving files in {directory} at port {port}")
        print(f"Download files by navigating to http://localhost:{port}")

        # 运行服务器，直到用户中断 (Ctrl+C)
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped by user.")


DEFAULT_DIRECTORY = os.getcwd()
DEFAULT_PORT = 8000

if __name__ == '__main__':
    if len(sys.argv) > 1:
        download_directory = sys.argv[1]
    else:
        download_directory = DEFAULT_DIRECTORY

    if len(sys.argv) > 2:
        ip_port = int(sys.argv[2])
    else:
        ip_port = DEFAULT_PORT

    if not os.path.isdir(download_directory):
        print(f"Error: {download_directory} is not a valid directory.")
        sys.exit(1)

    run(port=ip_port, directory=download_directory)
