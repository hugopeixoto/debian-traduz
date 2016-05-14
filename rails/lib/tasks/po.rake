namespace :po do
  desc "TODO"
  task :create, [:name, :path] => :environment do |t, args|
    r = Repository.create!(name: args[:name], path: File.expand_path(args[:path]))

    tr_st = ActiveRecord::Base.connection.raw_connection.prepare(
      "INSERT INTO translations (translation_file_id, msgid, msgstr, created_at, updated_at) VALUES(?, ?, ?, ?, ?);")

    # 2012-03-19 08:00:16.463415
    now = Time.now.strftime("%Y-%m-%d %H:%M:%S.%6N")

    %x[find #{r.path} -name "*.po" -printf "%P\n"].split("\n").each do |filepath|
      puts filepath
      file_id = r.translation_files.create(filepath: filepath).id

      GetPomo::PoFile.parse(File.read(r.path + "/" + filepath))[1..-1].each do |message|
        case message.msgid
          when String
            tr_st.execute(file_id, message.msgid, message.msgstr, now, now)
          when Array
            (0...message.size).each do |i|
              tr_st.execute(file_id, message[i].msgid, message[i].msgstr, now, now)
            end
          else
            raise Exception
        end
      end
    end

    tr_st.close()
  end

  desc "TODO"
  task :update => :environment do
    Repository.all.each do |repo|
      repo.update_translations
    end
  end

end
