from crawler import Crawler
from args import get_args


if __name__ == '__main__':
    args = get_args()
    crawler = Crawler()
    contents = crawler.crawl(args.start_date, args.end_date)
    with open(args.output, "w") as fp:
        for (date, title, content) in contents:
            title = title.replace('\n', '\r')
            title = title.replace('\"','\"\"')
            title = title.replace('\xa0','')
            content = content.replace('\n', '\r')
            content = content.replace('\"','\"\"')
            content = content.replace('\xa0','')
            out_str = f'"{str(date)}", "{title}", "{content}"\n'
            fp.write(out_str)
			

	 # TODO: write content to file according to spec
